#include "linear.h"
#include <iostream>

// создаёт пустую очередь
void InitQueue(Queue* queue) {
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->size = 0;
}

// освобождает все узлы
void FreeQueue(Queue* queue) {
    QNode* current = queue->head;
    while (current != nullptr) {
        QNode* tmp = current;
        current = current->next;
        delete tmp;
    }
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->size = 0;
}

// кладём значение в конец очереди
void QPUSH(Queue* queue, const std::string& value) {
    QNode* new_node = new QNode;
    new_node->data = value;
    new_node->next = nullptr;
    if (queue->tail == nullptr) {
        // очередь пуста - и голова, и хвост это новый узел
        queue->head = new_node;
        queue->tail = new_node;
    } else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    queue->size++;
}

// достаём значение из начала очереди
std::string QPOP(Queue* queue) {
    if (queue->head == nullptr) { std::cout << "no" << std::endl; return ""; }
    QNode* tmp = queue->head;
    std::string value = tmp->data;
    queue->head = queue->head->next;
    // если очередь опустела - обнуляем хвост
    if (queue->head == nullptr) queue->tail = nullptr;
    delete tmp;
    queue->size--;
    return value;
}

// достаём из начала, значение не возвращаем
void QPOP_VOID(Queue* queue) {
    if (queue->head == nullptr) { std::cout << "no" << std::endl; return; }
    QNode* tmp = queue->head;
    queue->head = queue->head->next;
    if (queue->head == nullptr) queue->tail = nullptr;
    delete tmp;
    queue->size--;
}

// печать очереди от головы к хвосту
void PRINT(Queue* queue) {
    std::cout << "Queue (head -> tail):" << std::endl;
    QNode* current = queue->head;
    while (current != nullptr) {
        std::cout << current->data << std::endl;
        current = current->next;
    }
}