#include "linear.h"
#include <iostream>

// создаёт пустой стек
void InitStack(Stack* stack) {
    stack->head = nullptr;
    stack->size = 0;
}

// освобождает все узлы
void FreeStack(Stack* stack) {
    SNode* current = stack->head;
    while (current != nullptr) {
        SNode* tmp = current;
        current = current->next;
        delete tmp;
    }
    stack->head = nullptr;
    stack->size = 0;
}

// кладём значение на верхушку (новый узел становится головой)
void SPUSH(Stack* stack, const std::string& value) {
    SNode* new_node = new SNode;
    new_node->data = value;
    new_node->next = stack->head;
    stack->head = new_node;
    stack->size++;
}

// снимаем верхушку и возвращаем её значение
std::string SPOP(Stack* stack) {
    if (stack->head == nullptr) { std::cout << "no" << std::endl; return ""; }
    SNode* tmp = stack->head;
    std::string value = tmp->data;
    stack->head = stack->head->next;
    delete tmp;
    stack->size--;
    return value;
}

// снимаем верхушку, значение не возвращаем
void SPOP_VOID(Stack* stack) {
    if (stack->head == nullptr) { std::cout << "no" << std::endl; return; }
    SNode* tmp = stack->head;
    stack->head = stack->head->next;
    delete tmp;
    stack->size--;
}

// печать стека сверху вниз
void PRINT(Stack* stack) {
    std::cout << "Stack (top -> bottom):" << std::endl;
    SNode* current = stack->head;
    while (current != nullptr) {
        std::cout << current->data << std::endl;
        current = current->next;
    }
}