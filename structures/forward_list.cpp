#include "linear.h"
#include <iostream>

// создаёт пустой список
void InitForwardList(ForwardList* list) {
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
}

// удаляет все узлы и обнуляет список
void FreeForwardList(ForwardList* list) {
    FNode* current = list->head;
    while (current != nullptr) {
        FNode* tmp = current;
        current = current->next;
        delete tmp;
    }
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
}

// вставка в голову
void FPUSH_FRONT(ForwardList* list, const std::string& value) {
    FNode* new_node = new FNode;
    new_node->data = value;
    new_node->next = list->head;
    list->head = new_node;
    // если список был пустой - хвост тоже новый узел
    if (list->tail == nullptr) list->tail = new_node;
    list->size++;
}

// вставка в хвост
void FPUSH_BACK(ForwardList* list, const std::string& value) {
    FNode* new_node = new FNode;
    new_node->data = value;
    new_node->next = nullptr;
    if (list->tail == nullptr) {
        // список пуст - и голова, и хвост смотрят на новый узел
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
}

// вставка после указанного узла
void FPUSH_AFTER(FNode* node, const std::string& value) {
    if (node == nullptr) return;
    FNode* new_node = new FNode;
    new_node->data = value;
    new_node->next = node->next;
    node->next = new_node;
}

// вставка перед указанным узлом
void FPUSH_BEFORE(ForwardList* list, FNode* node, const std::string& value) {
    if (node == nullptr) return;
    // если вставляем перед головой - это push front
    if (node == list->head) { FPUSH_FRONT(list, value); return; }
    // ищем узел, который стоит перед node
    FNode* prev = list->head;
    while (prev != nullptr && prev->next != node) prev = prev->next;
    if (prev == nullptr) return;
    FNode* new_node = new FNode;
    new_node->data = value;
    new_node->next = node;
    prev->next = new_node;
    list->size++;
}

// удалить голову
void FDEL_FRONT(ForwardList* list) {
    if (list->head == nullptr) { std::cout << "no" << std::endl; return; }
    FNode* tmp = list->head;
    list->head = list->head->next;
    delete tmp;
    list->size--;
    // если список стал пустым - обнуляем хвост
    if (list->head == nullptr) list->tail = nullptr;
}

// удалить хвост
void FDEL_BACK(ForwardList* list) {
    if (list->head == nullptr) { std::cout << "no" << std::endl; return; }
    // если элемент один - удаляем его и всё обнуляем
    if (list->head == list->tail) {
        delete list->head;
        list->head = nullptr;
        list->tail = nullptr;
        list->size = 0;
        return;
    }
    // ищем предпоследний узел
    FNode* prev = list->head;
    while (prev->next != list->tail) prev = prev->next;
    delete list->tail;
    prev->next = nullptr;
    list->tail = prev;
    list->size--;
}

// удалить узел после указанного
void FDEL_AFTER(ForwardList* list, FNode* node) {
    if (node == nullptr || node->next == nullptr) { std::cout << "no" << std::endl; return; }
    FNode* tmp = node->next;
    node->next = tmp->next;
    // если удалили хвост - переставляем tail
    if (tmp == list->tail) list->tail = node;
    delete tmp;
    list->size--;
}

// удалить узел перед указанным
void FDEL_BEFORE(ForwardList* list, FNode* node) {
    // нельзя удалить перед головой или вторым элементом
    if (node == nullptr || node == list->head || node == list->head->next) {
        std::cout << "no" << std::endl; return;
    }
    FNode* prev = list->head;
    FNode* prev_prev = nullptr;
    while (prev->next != node) {
        prev_prev = prev;
        prev = prev->next;
    }
    prev_prev->next = node;
    delete prev;
    list->size--;
}

// найти узел по значению
FNode* FFIND(ForwardList* list, const std::string& value) {
    FNode* current = list->head;
    while (current != nullptr) {
        if (current->data == value) return current;
        current = current->next;
    }
    return nullptr;
}

// удалить узел по значению
void FDEL_BY_VALUE(ForwardList* list, const std::string& value) {
    if (list->head == nullptr) { std::cout << "no" << std::endl; return; }
    // если голова содержит это значение - удаляем её
    if (list->head->data == value) { FDEL_FRONT(list); return; }
    FNode* prev = list->head;
    while (prev->next != nullptr && prev->next->data != value) prev = prev->next;
    if (prev->next == nullptr) { std::cout << "not found" << std::endl; return; }
    FNode* tmp = prev->next;
    prev->next = tmp->next;
    if (tmp == list->tail) list->tail = prev;
    delete tmp;
    list->size--;
}

// печать головы
void FGET_HEAD(ForwardList* list) {
    if (list->head == nullptr) { std::cout << "list is empty" << std::endl; return; }
    std::cout << "Head: " << list->head->data << std::endl;
}

// печать хвоста
void FGET_TAIL(ForwardList* list) {
    if (list->tail == nullptr) { std::cout << "list is empty" << std::endl; return; }
    std::cout << "Tail: " << list->tail->data << std::endl;
}

// рекурсивно идём до конца, печатаем на обратном ходу
void FGET_REVERSE(FNode* node) {
    if (node == nullptr) return;
    FGET_REVERSE(node->next);
    std::cout << node->data << std::endl;
}

// обычная печать списка
void PRINT(ForwardList* list) {
    std::cout << "ForwardList:" << std::endl;
    FNode* current = list->head;
    while (current != nullptr) {
        std::cout << current->data << std::endl;
        current = current->next;
    }
}