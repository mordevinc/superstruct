#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <vector>
#include "linear.h"
#include "avl_tree.h"

// тип структуры, чтобы знать с чем работать
enum StructType {
    TYPE_NONE = 0,
    TYPE_ARRAY,
    TYPE_FLIST,
    TYPE_DLIST,
    TYPE_STACK,
    TYPE_QUEUE,
    TYPE_DEQUE,
    TYPE_TREE
};

// одна структура с именем. внутри лежат все возможные поля,
// используется только то, что соответствует type
struct StructEntry {
    std::string name;
    StructType type = TYPE_NONE;

    Array array;
    ForwardList flist;
    DoublyList dlist;
    Stack stack;
    Queue queue;
    Deque deque;
    AVLTree tree;
};

// все структуры проекта
struct Registry {
    std::vector<StructEntry> entries;
};

// инициализация пустого реестра
void InitRegistry(Registry* reg);

// освобождение всех структур
void FreeRegistry(Registry* reg);

// создать структуру с именем и типом, вернуть указатель на неё
StructEntry* CreateStruct(Registry* reg, const std::string& name, StructType type);

// найти структуру по имени, nullptr если нет
StructEntry* FindStruct(Registry* reg, const std::string& name);

// перевод типа в строку и обратно
std::string TypeToString(StructType type);
StructType StringToType(const std::string& s);

// сохранить и загрузить весь реестр из файла
void SaveRegistry(Registry* reg, const std::string& filename);
void LoadRegistry(Registry* reg, const std::string& filename);

#endif