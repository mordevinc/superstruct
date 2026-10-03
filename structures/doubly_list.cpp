#include "linear.h"
#include <iostream>

// создаёт пустой двусвязный список
void InitDoublyList(DoublyList* list) {
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
}

// освобождает все узлы
void FreeDoublyList(DoublyList* list) {
    DNode* current = list->head;
    while (current != nullptr) {
        DNode* tmp = current;
        current = current->next;
        delete tmp;
    }
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
}

// вставка в голову
void LPUSH_FRONT(DoublyList* list, const std::string& value) {
    DNode* new_node = new DNode;
    new_node->data = value;
    new_node->prev = nullptr;
    new_node->next = list->head;
    // у старой головы prev теперь указывает на новый узел
    if (list->head != nullptr) list->head->prev = new_node;
    list->head = new_node;
    if (list->tail == nullptr) list->tail = new_node;
    list->size++;
}

// вставка в хвост
void LPUSH_BACK(DoublyList* list, const std::string& value) {
    DNode* new_node = new DNode;
    new_node->data = value;
    new_node->next = nullptr;
    new_node->prev = list->tail;
    // у старого хвоста next теперь указывает на новый узел
    if (list->tail != nullptr) list->tail->next = new_node;
    list->tail = new_node;
    if (list->head == nullptr) list->head = new_node;
    list->size++;
}

// вставка после узла
void LPUSH_AFTER(DoublyList* list, DNode* node, const std::string& value) {
    if (node == nullptr) return;
    if (node == list->tail) { LPUSH_BACK(list, value); return; }
    DNode* new_node = new DNode;
    new_node->data = value;
    new_node->prev = node;
    new_node->next = node->next;
    // перестраиваем связи соседей
    node->next->prev = new_node;
    node->next = new_node;
    list->size++;
}

// вставка перед узлом
void LPUSH_BEFORE(DoublyList* list, DNode* node, const std::string& value) {
    if (node == nullptr) return;
    if (node == list->head) { LPUSH_FRONT(list, value); return; }
    DNode* new_node = new DNode;
    new_node->data = value;
    new_node->next = node;
    new_node->prev = node->prev;
    node->prev->next = new_node;
    node->prev = new_node;
    list->size++;
}

// удалить голову
void LDEL_FRONT(DoublyList* list) {
    if (list->head == nullptr) { std::cout << "no" << std::endl; return; }
    DNode* tmp = list->head;
    list->head = list->head->next;
    if (list->head != nullptr) list->head->prev = nullptr;
    else list->tail = nullptr;
    delete tmp;
    list->size--;
}

// удалить хвост
void LDEL_BACK(DoublyList* list) {
    if (list->tail == nullptr) { std::cout << "no" << std::endl; return; }
    DNode* tmp = list->tail;
    list->tail = list->tail->prev;
    if (list->tail != nullptr) list->tail->next = nullptr;
    else list->head = nullptr;
    delete tmp;
    list->size--;
}

// удалить конкретный узел
void LDEL_NODE(DoublyList* list, DNode* node) {
    if (node == nullptr) return;
    if (node == list->head) { LDEL_FRONT(list); return; }
    if (node == list->tail) { LDEL_BACK(list); return; }
    // соседи теперь ссылаются друг на друга
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
    list->size--;
}

// удалить узел по значению
void LDEL_BY_VALUE(DoublyList* list, const std::string& value) {
    DNode* node = LFIND(list, value);
    if (node == nullptr) { std::cout << "not found" << std::endl; return; }
    LDEL_NODE(list, node);
}

// найти узел по значению
DNode* LFIND(DoublyList* list, const std::string& value) {
    DNode* current = list->head;
    while (current != nullptr) {
        if (current->data == value) return current;
        current = current->next;
    }
    return nullptr;
}

// печать головы
void LGET_HEAD(DoublyList* list) {
    if (list->head == nullptr) { std::cout << "list is empty" << std::endl; return; }
    std::cout << "Head: " << list->head->data << std::endl;
}

// печать хвоста
void LGET_TAIL(DoublyList* list) {
    if (list->tail == nullptr) { std::cout << "list is empty" << std::endl; return; }
    std::cout << "Tail: " << list->tail->data << std::endl;
}

// печать с конца, идём от tail назад по prev
void LGET_REVERSE(DoublyList* list) {
    DNode* current = list->tail;
    while (current != nullptr) {
        std::cout << current->data << std::endl;
        current = current->prev;
    }
}

// обычная печать списка
void PRINT(DoublyList* list) {
    std::cout << "DoublyList:" << std::endl;
    DNode* current = list->head;
    while (current != nullptr) {
        std::cout << current->data << std::endl;
        current = current->next;
    }
}