#include <iostream>
#include <sstream>
#include <vector>
#include "structures/registry.h"
#include "utils/flags.h"
#include "utils/json_io.h"
#include "utils/file_utils.h"

using namespace std;

// делим строку запроса по пробелам на отдельные слова
vector<string> SplitQuery(const string& query) {
    vector<string> tokens;
    string token = "";
    for (int i = 0; i < (int)query.size(); i++) {
        if (query[i] == ' ') {
            if (!token.empty()) { tokens.push_back(token); token = ""; }
        } else {
            token += query[i];
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

// разбирает одну команду пользователя и вызывает нужную операцию
void HandleQuery(Registry* reg, const string& query) {
    vector<string> t = SplitQuery(query);
    if (t.empty()) { cout << "empty query" << endl; return; }

    string cmd = t[0];

    // PRINT без имени печатает все структуры, с именем - только одну
    if (cmd == "PRINT") {
        if (t.size() < 2) {
            for (int i = 0; i < (int)reg->entries.size(); i++) {
                StructEntry* e = &reg->entries[i];
                cout << "=== " << e->name << " (" << TypeToString(e->type) << ") ===" << endl;
                if (e->type == TYPE_ARRAY) PRINT(&e->array);
                else if (e->type == TYPE_FLIST) PRINT(&e->flist);
                else if (e->type == TYPE_DLIST) PRINT(&e->dlist);
                else if (e->type == TYPE_STACK) PRINT(&e->stack);
                else if (e->type == TYPE_QUEUE) PRINT(&e->queue);
                else if (e->type == TYPE_DEQUE) PRINT(&e->deque);
                else if (e->type == TYPE_TREE) PRINT(&e->tree);
            }
            return;
        }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e) { cout << "not found" << endl; return; }
        if (e->type == TYPE_ARRAY) PRINT(&e->array);
        else if (e->type == TYPE_FLIST) PRINT(&e->flist);
        else if (e->type == TYPE_DLIST) PRINT(&e->dlist);
        else if (e->type == TYPE_STACK) PRINT(&e->stack);
        else if (e->type == TYPE_QUEUE) PRINT(&e->queue);
        else if (e->type == TYPE_DEQUE) PRINT(&e->deque);
        else if (e->type == TYPE_TREE) PRINT(&e->tree);
        return;
    }

    // CREATE имя тип - создать новую структуру
    if (cmd == "CREATE") {
        if (t.size() < 3) { cout << "usage: CREATE <name> <type>" << endl; return; }
        StructType type = StringToType(t[2]);
        if (type == TYPE_NONE) { cout << "unknown type" << endl; return; }
        if (FindStruct(reg, t[1])) { cout << "already exists" << endl; return; }
        CreateStruct(reg, t[1], type);
        cout << "-> created" << endl;
        return;
    }

    // MPUSH имя значение - добавить в конец массива
    if (cmd == "MPUSH") {
        if (t.size() < 3) { cout << "usage: MPUSH <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_ARRAY) { cout << "not found" << endl; return; }
        MPUSH(&e->array, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "MPUSH_AT") {
        if (t.size() < 4) { cout << "usage: MPUSH_AT <name> <index> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_ARRAY) { cout << "not found" << endl; return; }
        MPUSH_AT(&e->array, stoi(t[2]), t[3]);
        cout << "-> " << t[3] << endl;
    }
    else if (cmd == "MDEL") {
        if (t.size() < 3) { cout << "usage: MDEL <name> <index>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_ARRAY) { cout << "not found" << endl; return; }
        MDEL(&e->array, stoi(t[2]));
        cout << "-> OK" << endl;
    }
    else if (cmd == "MGET") {
        if (t.size() < 3) { cout << "usage: MGET <name> <index>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_ARRAY) { cout << "not found" << endl; return; }
        cout << "-> " << MGET(&e->array, stoi(t[2])) << endl;
    }
    else if (cmd == "MSET") {
        if (t.size() < 4) { cout << "usage: MSET <name> <index> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_ARRAY) { cout << "not found" << endl; return; }
        MSET(&e->array, stoi(t[2]), t[3]);
        cout << "-> OK" << endl;
    }
    else if (cmd == "MLEN") {
        if (t.size() < 2) { cout << "usage: MLEN <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_ARRAY) { cout << "not found" << endl; return; }
        cout << "-> " << MLEN(&e->array) << endl;
    }

    // FPUSH имя значение - добавить в хвост односвязного
    else if (cmd == "FPUSH") {
        if (t.size() < 3) { cout << "usage: FPUSH <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FPUSH_BACK(&e->flist, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "FPUSH_FRONT") {
        if (t.size() < 3) { cout << "usage: FPUSH_FRONT <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FPUSH_FRONT(&e->flist, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "FDEL") {
        if (t.size() < 2) { cout << "usage: FDEL <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FDEL_FRONT(&e->flist);
        cout << "-> OK" << endl;
    }
    else if (cmd == "FGET") {
        if (t.size() < 2) { cout << "usage: FGET <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FGET_HEAD(&e->flist);
    }

    // LPUSH имя значение - добавить в хвост двусвязного
    else if (cmd == "LPUSH") {
        if (t.size() < 3) { cout << "usage: LPUSH <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LPUSH_BACK(&e->dlist, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "LPUSH_FRONT") {
        if (t.size() < 3) { cout << "usage: LPUSH_FRONT <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LPUSH_FRONT(&e->dlist, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "LDEL") {
        if (t.size() < 2) { cout << "usage: LDEL <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LDEL_FRONT(&e->dlist);
        cout << "-> OK" << endl;
    }
    else if (cmd == "LGET") {
        if (t.size() < 2) { cout << "usage: LGET <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LGET_HEAD(&e->dlist);
    }

    // SPUSH имя значение - положить в стек
    else if (cmd == "SPUSH") {
        if (t.size() < 3) { cout << "usage: SPUSH <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_STACK) { cout << "not found" << endl; return; }
        SPUSH(&e->stack, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "SPOP") {
        if (t.size() < 2) { cout << "usage: SPOP <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_STACK) { cout << "not found" << endl; return; }
        cout << "-> " << SPOP(&e->stack) << endl;
    }

    // QPUSH имя значение - положить в конец очереди
    else if (cmd == "QPUSH") {
        if (t.size() < 3) { cout << "usage: QPUSH <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_QUEUE) { cout << "not found" << endl; return; }
        QPUSH(&e->queue, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "QPOP") {
        if (t.size() < 2) { cout << "usage: QPOP <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_QUEUE) { cout << "not found" << endl; return; }
        cout << "-> " << QPOP(&e->queue) << endl;
    }

    // DPUSH_BACK имя значение - положить в конец двусвязной очереди
    else if (cmd == "DPUSH_BACK") {
        if (t.size() < 3) { cout << "usage: DPUSH_BACK <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        DPUSH_BACK(&e->deque, t[2]);
        cout << "-> " << t[2] << endl;
    }
    // DPUSH_FRONT имя значение - положить в начало
    else if (cmd == "DPUSH_FRONT") {
        if (t.size() < 3) { cout << "usage: DPUSH_FRONT <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        DPUSH_FRONT(&e->deque, t[2]);
        cout << "-> " << t[2] << endl;
    }
    // DPUSH — по умолчанию в конец (как у очереди)
    else if (cmd == "DPUSH") {
        if (t.size() < 3) { cout << "usage: DPUSH <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        DPUSH_BACK(&e->deque, t[2]);
        cout << "-> " << t[2] << endl;
    }
    // DPOP_FRONT имя - достать из начала
    else if (cmd == "DPOP_FRONT") {
        if (t.size() < 2) { cout << "usage: DPOP_FRONT <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        cout << "-> " << DPOP_FRONT(&e->deque) << endl;
    }
    // DPOP_BACK имя - достать из конца
    else if (cmd == "DPOP_BACK") {
        if (t.size() < 2) { cout << "usage: DPOP_BACK <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        cout << "-> " << DPOP_BACK(&e->deque) << endl;
    }
    // DPOP — по умолчанию из начала (как у очереди)
    else if (cmd == "DPOP") {
        if (t.size() < 2) { cout << "usage: DPOP <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        cout << "-> " << DPOP_FRONT(&e->deque) << endl;
    }
    // DGET имя - прочитать начало
    else if (cmd == "DGET") {
        if (t.size() < 2) { cout << "usage: DGET <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        DGET_HEAD(&e->deque);
    }
    else if (cmd == "DGET_HEAD") {
        if (t.size() < 2) { cout << "usage: DGET_HEAD <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        DGET_HEAD(&e->deque);
    }
    else if (cmd == "DGET_TAIL") {
        if (t.size() < 2) { cout << "usage: DGET_TAIL <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        DGET_TAIL(&e->deque);
    }

    // TINSERT имя ключ - вставить ключ в дерево (ключ - число)
    else if (cmd == "TINSERT") {
        if (t.size() < 3) { cout << "usage: TINSERT <name> <key>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_TREE) { cout << "not found" << endl; return; }
        TINSERT(&e->tree, stoi(t[2]));
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "TDEL") {
        if (t.size() < 3) { cout << "usage: TDEL <name> <key>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_TREE) { cout << "not found" << endl; return; }
        TDEL(&e->tree, stoi(t[2]));
        cout << "-> OK" << endl;
    }
    else if (cmd == "TGET") {
        if (t.size() < 3) { cout << "usage: TGET <name> <key>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_TREE) { cout << "not found" << endl; return; }
        AVLNode* found = TFIND(&e->tree, stoi(t[2]));
        cout << "-> " << (found ? "TRUE" : "FALSE") << endl;
    }

    else {
        cout << "unknown command: " << cmd << endl;
    }
}

// точка входа: разбирает флаги, грузит реестр, обрабатывает запрос, сохраняет
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");

    EnsureDirectory("data");
    EnsureDirectory("bin");

    Flags flags;
    ParseFlags(argc, argv, &flags);

    string filename = GetFlag(&flags, "--file");
    if (filename.empty()) filename = "data.json";
    filename = BuildDataPath(filename);

    Registry reg;
    InitRegistry(&reg);

    // если файл есть - грузим, если нет - создаём дефолтные структуры
    if (FileExists(filename)) {
        LoadRegistry(&reg, filename);
    } else {
        CreateStruct(&reg, "myarray", TYPE_ARRAY);
        CreateStruct(&reg, "mylist", TYPE_FLIST);
        CreateStruct(&reg, "mydlist", TYPE_DLIST);
        CreateStruct(&reg, "mystack", TYPE_STACK);
        CreateStruct(&reg, "myqueue", TYPE_QUEUE);
        CreateStruct(&reg, "mydeque", TYPE_DEQUE);
        CreateStruct(&reg, "mytree", TYPE_TREE);
        SaveRegistry(&reg, filename);
    }

    string query = GetFlag(&flags, "--query");
    if (!query.empty()) {
        HandleQuery(&reg, query);
    } else {
        HandleQuery(&reg, "PRINT");
    }

    SaveRegistry(&reg, filename);

    FreeRegistry(&reg);
    return 0;
}