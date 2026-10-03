#include "registry.h"
#include "../utils/json_io.h"
#include <iostream>

// реестр изначально пустой
void InitRegistry(Registry* reg) {
    reg->entries.clear();
}

// освобождаем каждую структуру в зависимости от её типа
void FreeRegistry(Registry* reg) {
    for (int i = 0; i < (int)reg->entries.size(); i++) {
        StructEntry& e = reg->entries[i];
        if (e.type == TYPE_ARRAY) FreeArray(&e.array);
        else if (e.type == TYPE_FLIST) FreeForwardList(&e.flist);
        else if (e.type == TYPE_DLIST) FreeDoublyList(&e.dlist);
        else if (e.type == TYPE_STACK) FreeStack(&e.stack);
        else if (e.type == TYPE_QUEUE) FreeQueue(&e.queue);
        else if (e.type == TYPE_DEQUE) FreeDeque(&e.deque);
        else if (e.type == TYPE_TREE) FreeAVLTree(&e.tree);
    }
    reg->entries.clear();
}

// создаёт структуру, кладёт её в вектор и возвращает указатель на копию
StructEntry* CreateStruct(Registry* reg, const std::string& name, StructType type) {
    StructEntry e;
    e.name = name;
    e.type = type;

    if (type == TYPE_ARRAY) InitArray(&e.array, 100);
    else if (type == TYPE_FLIST) InitForwardList(&e.flist);
    else if (type == TYPE_DLIST) InitDoublyList(&e.dlist);
    else if (type == TYPE_STACK) InitStack(&e.stack);
    else if (type == TYPE_QUEUE) InitQueue(&e.queue);
    else if (type == TYPE_DEQUE) InitDeque(&e.deque);
    else if (type == TYPE_TREE) InitAVLTree(&e.tree);

    reg->entries.push_back(e);
    return &reg->entries.back();
}

// поиск по имени
StructEntry* FindStruct(Registry* reg, const std::string& name) {
    for (int i = 0; i < (int)reg->entries.size(); i++) {
        if (reg->entries[i].name == name) return &reg->entries[i];
    }
    return nullptr;
}

// перевод enum в строку для записи в файл
std::string TypeToString(StructType type) {
    if (type == TYPE_ARRAY) return "array";
    if (type == TYPE_FLIST) return "flist";
    if (type == TYPE_DLIST) return "dlist";
    if (type == TYPE_STACK) return "stack";
    if (type == TYPE_QUEUE) return "queue";
    if (type == TYPE_DEQUE) return "deque";
    if (type == TYPE_TREE) return "tree";
    return "none";
}

// перевод строки из файла обратно в enum
StructType StringToType(const std::string& s) {
    if (s == "array") return TYPE_ARRAY;
    if (s == "flist") return TYPE_FLIST;
    if (s == "dlist") return TYPE_DLIST;
    if (s == "stack") return TYPE_STACK;
    if (s == "queue") return TYPE_QUEUE;
    if (s == "deque") return TYPE_DEQUE;
    if (s == "tree") return TYPE_TREE;
    return TYPE_NONE;
}

// превращает содержимое структуры в строку "a,b,c,"
std::string SerializeEntry(StructEntry* e) {
    std::string data = "";

    if (e->type == TYPE_ARRAY) {
        for (int i = 0; i < e->array.size; i++) {
            data += e->array.data[i] + ",";
        }
    } else if (e->type == TYPE_FLIST) {
        FNode* cur = e->flist.head;
        while (cur) { data += cur->data + ","; cur = cur->next; }
    } else if (e->type == TYPE_DLIST) {
        DNode* cur = e->dlist.head;
        while (cur) { data += cur->data + ","; cur = cur->next; }
    } else if (e->type == TYPE_STACK) {
        SNode* cur = e->stack.head;
        while (cur) { data += cur->data + ","; cur = cur->next; }
    } else if (e->type == TYPE_QUEUE) {
        QNode* cur = e->queue.head;
        while (cur) { data += cur->data + ","; cur = cur->next; }
    } else if (e->type == TYPE_DEQUE) {
        // сохраняем от головы к хвосту
        DNode* cur = e->deque.head;
        while (cur) { data += cur->data + ","; cur = cur->next; }
    } else if (e->type == TYPE_TREE) {
        CollectAVLKeys(e->tree.root, data);
    }

    return data;
}

// разбирает строку "a,b,c," и кладёт значения обратно
void DeserializeEntry(StructEntry* e, const std::string& data) {
    std::string item = "";
    for (int i = 0; i < (int)data.size(); i++) {
        if (data[i] == ',') {
            if (!item.empty()) {
                if (e->type == TYPE_ARRAY) MPUSH(&e->array, item);
                else if (e->type == TYPE_FLIST) FPUSH_BACK(&e->flist, item);
                else if (e->type == TYPE_DLIST) LPUSH_BACK(&e->dlist, item);
                else if (e->type == TYPE_STACK) SPUSH(&e->stack, item);
                else if (e->type == TYPE_QUEUE) QPUSH(&e->queue, item);
                // deque восстанавливаем в том же порядке: назад, в хвост
                else if (e->type == TYPE_DEQUE) DPUSH_BACK(&e->deque, item);
                else if (e->type == TYPE_TREE) TINSERT(&e->tree, stoi(item));
                item = "";
            }
        } else {
            item += data[i];
        }
    }
}

// сохраняет все структуры реестра в json-файл
void SaveRegistry(Registry* reg, const std::string& filename) {
    JsonObject obj = JsonCreate();
    for (int i = 0; i < (int)reg->entries.size(); i++) {
        StructEntry* e = &reg->entries[i];
        std::string key = e->name + "|" + TypeToString(e->type);
        JsonSet(&obj, key, SerializeEntry(e));
    }
    JsonWriteToFile(&obj, filename);
}

// загружает реестр из файла
void LoadRegistry(Registry* reg, const std::string& filename) {
    JsonObject obj = JsonReadFromFile(filename);
    for (int i = 0; i < (int)obj.pairs.size(); i++) {
        std::string full = obj.pairs[i].key;
        std::string data = obj.pairs[i].value;

        size_t sep = full.find('|');
        if (sep == std::string::npos) continue;

        std::string name = full.substr(0, sep);
        std::string type = full.substr(sep + 1);

        StructType t = StringToType(type);
        if (t == TYPE_NONE) continue;

        StructEntry* e = CreateStruct(reg, name, t);
        DeserializeEntry(e, data);
    }
}