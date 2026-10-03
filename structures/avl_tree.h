#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <string>

// узел АВЛ-дерева: хранит целочисленный ключ, детей и высоту поддерева
struct AVLNode {
    AVLNode* left = nullptr;
    AVLNode* right = nullptr;
    int height = 1;
    int key = 0;
};

// само дерево
struct AVLTree {
    int size = 0;
    AVLNode* root = nullptr;
};

// создать пустое дерево
void InitAVLTree(AVLTree* tree);

// освободить все узлы
void FreeAVLTree(AVLTree* tree);

// вставить ключ, дерево само себя балансирует
void TINSERT(AVLTree* tree, int key);

// удалить ключ с балансировкой
void TDEL(AVLTree* tree, int key);

// найти узел по ключу, nullptr если нет
AVLNode* TFIND(AVLTree* tree, int key);

// симметричный обход (по возрастанию)
void PRINT(AVLTree* tree);

// прямой обход (корень, левое, правое)
void TGET_PREORDER(AVLTree* tree);

// обратный обход (левое, правое, корень)
void TGET_POSTORDER(AVLTree* tree);

// обход в ширину по уровням
void TGET_BFS(AVLTree* tree);

// собрать все ключи дерева в строку через запятую (для сохранения)
void CollectAVLKeys(AVLNode* node, std::string& out);

#endif