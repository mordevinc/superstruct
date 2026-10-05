#ifndef LINEAR_H
#define LINEAR_H

#include <string>

// массив фиксированной вместимости
struct Array {
    int size = 0;
    int capacity = 0;
    std::string* data = nullptr;
};

// создать массив с заданной вместимостью
void InitArray(Array* arr, int capacity);

// освободить память массива
void FreeArray(Array* arr);

// добавить элемент в конец массива
void MPUSH(Array* arr, const std::string& value);

// вставить элемент по индексу (сдвигает остальные вправо)
void MPUSH_AT(Array* arr, int index, const std::string& value);

// удалить элемент по индексу (сдвигает остальные влево)
void MDEL(Array* arr, int index);

// получить элемент по индексу
std::string MGET(Array* arr, int index);

// заменить элемент по индексу
void MSET(Array* arr, int index, const std::string& value);

// вернуть текущее количество элементов
int MLEN(Array* arr);

// узел односвязного списка
struct FNode {
    FNode* next = nullptr;
    std::string data;
};

// сам односвязный список с указателями на голову и хвост
struct ForwardList {
    int size = 0;
    FNode* head = nullptr;
    FNode* tail = nullptr;
};

// создать пустой список
void InitForwardList(ForwardList* list);

// освободить все узлы
void FreeForwardList(ForwardList* list);

// добавить в голову списка
void FPUSH_FRONT(ForwardList* list, const std::string& value);

// добавить в хвост списка
void FPUSH_BACK(ForwardList* list, const std::string& value);

// вставить после указанного узла
void FPUSH_AFTER(FNode* node, const std::string& value);

// вставить перед указанным узлом
void FPUSH_BEFORE(ForwardList* list, FNode* node, const std::string& value);

// удалить голову
void FDEL_FRONT(ForwardList* list);

// удалить хвост
void FDEL_BACK(ForwardList* list);

// удалить узел после указанного
void FDEL_AFTER(ForwardList* list, FNode* node);

// удалить узел перед указанным
void FDEL_BEFORE(ForwardList* list, FNode* node);

// найти узел по значению, вернуть nullptr если нет
FNode* FFIND(ForwardList* list, const std::string& value);

// удалить узел по значению
void FDEL_BY_VALUE(ForwardList* list, const std::string& value);

// вывести значение головы
void FGET_HEAD(ForwardList* list);

// вывести значение хвоста
void FGET_TAIL(ForwardList* list);

// рекурсивно вывести список с конца
void FGET_REVERSE(FNode* node);

// узел двусвязного списка (используется и в двусвязной очереди)
struct DNode {
    DNode* prev = nullptr;
    DNode* next = nullptr;
    std::string data;
};

// сам двусвязный список
struct DoublyList {
    int size = 0;
    DNode* head = nullptr;
    DNode* tail = nullptr;
};

// создать пустой список
void InitDoublyList(DoublyList* list);

// освободить все узлы
void FreeDoublyList(DoublyList* list);

// добавить в голову
void LPUSH_FRONT(DoublyList* list, const std::string& value);

// добавить в хвост
void LPUSH_BACK(DoublyList* list, const std::string& value);

// вставить после узла
void LPUSH_AFTER(DoublyList* list, DNode* node, const std::string& value);

// вставить перед узлом
void LPUSH_BEFORE(DoublyList* list, DNode* node, const std::string& value);

// удалить голову
void LDEL_FRONT(DoublyList* list);

// удалить хвост
void LDEL_BACK(DoublyList* list);

// удалить конкретный узел
void LDEL_NODE(DoublyList* list, DNode* node);

// удалить узел после указанного
void LDEL_AFTER(DoublyList* list, DNode* node);

// удалить узел перед указанным
void LDEL_BEFORE(DoublyList* list, DNode* node);

// удалить по значению
void LDEL_BY_VALUE(DoublyList* list, const std::string& value);

// найти узел по значению
DNode* LFIND(DoublyList* list, const std::string& value);

// вывести значение головы
void LGET_HEAD(DoublyList* list);

// вывести значение хвоста
void LGET_TAIL(DoublyList* list);

// вывести список в обратном порядке
void LGET_REVERSE(DoublyList* list);

// узел стека
struct SNode {
    SNode* next = nullptr;
    std::string data;
};

// сам стек
struct Stack {
    int size = 0;
    SNode* head = nullptr;
};

// создать пустой стек
void InitStack(Stack* stack);

// освободить память
void FreeStack(Stack* stack);

// положить значение на верхушку стека
void SPUSH(Stack* stack, const std::string& value);

// снять верхушку и вернуть значение
std::string SPOP(Stack* stack);

// снять верхушку без возврата значения
void SPOP_VOID(Stack* stack);

// узел очереди
struct QNode {
    QNode* next = nullptr;
    std::string data;
};

// сама очередь
struct Queue {
    int size = 0;
    QNode* head = nullptr;
    QNode* tail = nullptr;
};

// создать пустую очередь
void InitQueue(Queue* queue);

// освободить память
void FreeQueue(Queue* queue);

// положить значение в конец очереди
void QPUSH(Queue* queue, const std::string& value);

// достать значение из начала и вернуть его
std::string QPOP(Queue* queue);

// достать из начала без возврата
void QPOP_VOID(Queue* queue);

// сама двусвязная очередь, узлы берутся от DNode
struct Deque {
    int size = 0;
    DNode* head = nullptr;
    DNode* tail = nullptr;
};

// создать пустую двусвязную очередь
void InitDeque(Deque* deque);

// освободить все узлы
void FreeDeque(Deque* deque);

// положить значение в начало
void DPUSH_FRONT(Deque* deque, const std::string& value);

// положить значение в конец
void DPUSH_BACK(Deque* deque, const std::string& value);

// достать значение из начала
std::string DPOP_FRONT(Deque* deque);

// достать значение из конца
std::string DPOP_BACK(Deque* deque);

// прочитать начало
void DGET_HEAD(Deque* deque);

// прочитать конец
void DGET_TAIL(Deque* deque);

// вывод структуры на экран
void PRINT(Array* arr);
void PRINT(ForwardList* list);
void PRINT(DoublyList* list);
void PRINT(Stack* stack);
void PRINT(Queue* queue);
void PRINT(Deque* deque);

#endif