#include "linear.h"
#include <iostream>

// создаёт пустую двусвязную очередь
void InitDeque(Deque* deque) {
    deque->head = nullptr;
    deque->tail = nullptr;
    deque->size = 0;
}

// освобождает все узлы
void FreeDeque(Deque* deque) {
    DNode* current = deque->head;
    while (current != nullptr) {
        DNode* tmp = current;
        current = current->next;
        delete tmp;
    }
    deque->head = nullptr;
    deque->tail = nullptr;
    deque->size = 0;
}

// вставка в начало: новый узел становится головой
void DPUSH_FRONT(Deque* deque, const std::string& value) {
    DNode* new_node = new DNode;
    new_node->data = value;
    new_node->prev = nullptr;
    new_node->next = deque->head;
    if (deque->head != nullptr) deque->head->prev = new_node;
    deque->head = new_node;
    if (deque->tail == nullptr) deque->tail = new_node;
    deque->size++;
}

// вставка в конец: новый узел становится хвостом
void DPUSH_BACK(Deque* deque, const std::string& value) {
    DNode* new_node = new DNode;
    new_node->data = value;
    new_node->next = nullptr;
    new_node->prev = deque->tail;
    if (deque->tail != nullptr) deque->tail->next = new_node;
    deque->tail = new_node;
    if (deque->head == nullptr) deque->head = new_node;
    deque->size++;
}

// удаляем голову и возвращаем её значение
std::string DPOP_FRONT(Deque* deque) {
    if (deque->head == nullptr) { std::cout << "no" << std::endl; return ""; }
    DNode* tmp = deque->head;
    std::string value = tmp->data;
    deque->head = deque->head->next;
    if (deque->head != nullptr) deque->head->prev = nullptr;
    else deque->tail = nullptr;
    delete tmp;
    deque->size--;
    return value;
}

// удаляем хвост и возвращаем его значение
std::string DPOP_BACK(Deque* deque) {
    if (deque->tail == nullptr) { std::cout << "no" << std::endl; return ""; }
    DNode* tmp = deque->tail;
    std::string value = tmp->data;
    deque->tail = deque->tail->prev;
    if (deque->tail != nullptr) deque->tail->next = nullptr;
    else deque->head = nullptr;
    delete tmp;
    deque->size--;
    return value;
}

// печать головы
void DGET_HEAD(Deque* deque) {
    if (deque->head == nullptr) { std::cout << "deque is empty" << std::endl; return; }
    std::cout << "Head: " << deque->head->data << std::endl;
}

// печать хвоста
void DGET_TAIL(Deque* deque) {
    if (deque->tail == nullptr) { std::cout << "deque is empty" << std::endl; return; }
    std::cout << "Tail: " << deque->tail->data << std::endl;
}

// печать от головы к хвосту
void PRINT(Deque* deque) {
    std::cout << "Deque:" << std::endl;
    DNode* current = deque->head;
    while (current != nullptr) {
        std::cout << current->data << std::endl;
        current = current->next;
    }
}