#include "linear.h"
#include <iostream>

// выделяет память под массив строк
void InitArray(Array* arr, int capacity) {
    arr->size = 0;
    arr->capacity = capacity;
    arr->data = new std::string[capacity];
}

// освобождает память массива
void FreeArray(Array* arr) {
    delete[] arr->data;
    arr->data = nullptr;
    arr->size = 0;
    arr->capacity = 0;
}

// добавить значение в конец, если есть место
void MPUSH(Array* arr, const std::string& value) {
    if (arr->size >= arr->capacity) {
        std::cout << "Array is full" << std::endl;
        return;
    }
    arr->data[arr->size] = value;
    arr->size++;
}

// вставить значение по индексу со сдвигом вправо
void MPUSH_AT(Array* arr, int index, const std::string& value) {
    if (index < 0 || index > arr->size || arr->size >= arr->capacity) {
        std::cout << "Bad index or full" << std::endl;
        return;
    }
    // идём с конца, сдвигая каждый элемент на одну позицию вправо
    for (int i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i - 1];
    }
    arr->data[index] = value;
    arr->size++;
}

// удалить значение по индексу со сдвигом влево
void MDEL(Array* arr, int index) {
    if (index < 0 || index >= arr->size) {
        std::cout << "Bad index" << std::endl;
        return;
    }
    for (int i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
}

// получить значение по индексу
std::string MGET(Array* arr, int index) {
    if (index < 0 || index >= arr->size) return "";
    return arr->data[index];
}

// заменить значение по индексу
void MSET(Array* arr, int index, const std::string& value) {
    if (index < 0 || index >= arr->size) {
        std::cout << "Bad index" << std::endl;
        return;
    }
    arr->data[index] = value;
}

// длина массива
int MLEN(Array* arr) {
    return arr->size;
}

// печать массива в квадратных скобках
void PRINT(Array* arr) {
    std::cout << "[ ";
    for (int i = 0; i < arr->size; i++) {
        std::cout << arr->data[i] << " ";
    }
    std::cout << "]" << std::endl;
}