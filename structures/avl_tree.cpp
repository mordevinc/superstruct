#include "avl_tree.h"
#include <iostream>
#include <queue>

// высота узла, для nullptr = 0
int Height(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

// разница высот левого и правого поддерева
int BalanceFactor(AVLNode* node) {
    if (node == nullptr) return 0;
    return Height(node->left) - Height(node->right);
}

// пересчитывает высоту узла по детям
void UpdateHeight(AVLNode* node) {
    if (node == nullptr) return;
    int hl = Height(node->left);
    int hr = Height(node->right);
    node->height = (hl > hr ? hl : hr) + 1;
}

// малое правое вращение, нужно когда левый перевес
AVLNode* RotateRight(AVLNode* a) {
    AVLNode* b = a->left;
    a->left = b->right;
    b->right = a;
    UpdateHeight(a);
    UpdateHeight(b);
    return b;
}

// малое левое вращение, нужно когда правый перевес
AVLNode* RotateLeft(AVLNode* a) {
    AVLNode* b = a->right;
    a->right = b->left;
    b->left = a;
    UpdateHeight(a);
    UpdateHeight(b);
    return b;
}

// балансирует узел: если разница высот больше 1 - делает повороты
AVLNode* Balance(AVLNode* node) {
    UpdateHeight(node);
    int bf = BalanceFactor(node);

    // левый перевес
    if (bf > 1) {
        // если у левого ребёнка правый перевес - сначала левое вращение
        if (BalanceFactor(node->left) < 0) {
            node->left = RotateLeft(node->left);
        }
        return RotateRight(node);
    }

    // правый перевес
    if (bf < -1) {
        // если у правого ребёнка левый перевес - сначала правое вращение
        if (BalanceFactor(node->right) > 0) {
            node->right = RotateRight(node->right);
        }
        return RotateLeft(node);
    }

    return node;
}

// вставка узла как в обычном BST, потом балансировка
AVLNode* InsertNode(AVLNode* node, int key) {
    if (node == nullptr) {
        AVLNode* new_node = new AVLNode;
        new_node->key = key;
        return new_node;
    }
    if (key < node->key) node->left = InsertNode(node->left, key);
    else if (key > node->key) node->right = InsertNode(node->right, key);
    else return node;   // дубликат игнорируем
    return Balance(node);
}

// публичная вставка
void TINSERT(AVLTree* tree, int key) {
    tree->root = InsertNode(tree->root, key);
    tree->size++;
}

// найти самый левый узел в поддереве
AVLNode* FindMin(AVLNode* node) {
    while (node != nullptr && node->left != nullptr) node = node->left;
    return node;
}

// рекурсивное удаление с балансировкой
AVLNode* DeleteNode(AVLNode* node, int key) {
    if (node == nullptr) return nullptr;

    if (key < node->key) {
        node->left = DeleteNode(node->left, key);
    } else if (key > node->key) {
        node->right = DeleteNode(node->right, key);
    } else {
        // если один или ноль детей - заменяем на ребёнка
        if (node->left == nullptr || node->right == nullptr) {
            AVLNode* child = node->left ? node->left : node->right;
            if (child == nullptr) {
                delete node;
                return nullptr;
            } else {
                AVLNode* tmp = node;
                node = child;
                delete tmp;
            }
        } else {
            // два ребёнка: берём минимум из правого поддерева
            AVLNode* min_node = FindMin(node->right);
            node->key = min_node->key;
            node->right = DeleteNode(node->right, min_node->key);
        }
    }
    return Balance(node);
}

// публичное удаление
void TDEL(AVLTree* tree, int key) {
    if (TFIND(tree, key) == nullptr) {
        std::cout << "not found" << std::endl;
        return;
    }
    tree->root = DeleteNode(tree->root, key);
    tree->size--;
}

// рекурсивный поиск по BST-правилу
AVLNode* FindNode(AVLNode* node, int key) {
    if (node == nullptr) return nullptr;
    if (key == node->key) return node;
    if (key < node->key) return FindNode(node->left, key);
    return FindNode(node->right, key);
}

// публичный поиск
AVLNode* TFIND(AVLTree* tree, int key) {
    return FindNode(tree->root, key);
}

// симметричный обход: левое, корень, правое
void InorderPrint(AVLNode* node) {
    if (node == nullptr) return;
    InorderPrint(node->left);
    std::cout << node->key << std::endl;
    InorderPrint(node->right);
}

// печать симметричным обходом
void PRINT(AVLTree* tree) {
    std::cout << "AVLTree (inorder):" << std::endl;
    InorderPrint(tree->root);
}

// прямой обход: корень, левое, правое
void PreorderPrint(AVLNode* node) {
    if (node == nullptr) return;
    std::cout << node->key << std::endl;
    PreorderPrint(node->left);
    PreorderPrint(node->right);
}

// печать прямым обходом
void TGET_PREORDER(AVLTree* tree) {
    std::cout << "AVLTree (preorder):" << std::endl;
    PreorderPrint(tree->root);
}

// обратный обход: левое, правое, корень
void PostorderPrint(AVLNode* node) {
    if (node == nullptr) return;
    PostorderPrint(node->left);
    PostorderPrint(node->right);
    std::cout << node->key << std::endl;
}

// печать обратным обходом
void TGET_POSTORDER(AVLTree* tree) {
    std::cout << "AVLTree (postorder):" << std::endl;
    PostorderPrint(tree->root);
}

// обход в ширину через очередь
void TGET_BFS(AVLTree* tree) {
    std::cout << "AVLTree (BFS):" << std::endl;
    if (tree->root == nullptr) return;
    std::queue<AVLNode*> q;
    q.push(tree->root);
    while (!q.empty()) {
        AVLNode* current = q.front();
        q.pop();
        std::cout << current->key << std::endl;
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
}

// собирает все ключи дерева в строку через запятую (число переводим в строку)
void CollectAVLKeys(AVLNode* node, std::string& out) {
    if (node == nullptr) return;
    out += std::to_string(node->key) + ",";
    CollectAVLKeys(node->left, out);
    CollectAVLKeys(node->right, out);
}

// рекурсивно освобождает все узлы
void FreeNodes(AVLNode* node) {
    if (node == nullptr) return;
    FreeNodes(node->left);
    FreeNodes(node->right);
    delete node;
}

// создать пустое дерево
void InitAVLTree(AVLTree* tree) {
    tree->root = nullptr;
    tree->size = 0;
}

// освободить всё дерево
void FreeAVLTree(AVLTree* tree) {
    FreeNodes(tree->root);
    tree->root = nullptr;
    tree->size = 0;
}