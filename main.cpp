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

    // МАССИВ
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

    // ОДНОСВЯЗНЫЙ СПИСОК 
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
    // FPUSH_AFTER <имя> <ориентир> <значение> - вставить после узла с данным значением
    else if (cmd == "FPUSH_AFTER") {
        if (t.size() < 4) { cout << "usage: FPUSH_AFTER <name> <anchor> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FNode* anchor = FFIND(&e->flist, t[2]);
        if (anchor == nullptr) { cout << "anchor not found" << endl; return; }
        FPUSH_AFTER(anchor, t[3]);
        e->flist.size++;
        cout << "-> " << t[3] << endl;
    }
    // FPUSH_BEFORE <имя> <ориентир> <значение> - вставить перед узлом с данным значением
    else if (cmd == "FPUSH_BEFORE") {
        if (t.size() < 4) { cout << "usage: FPUSH_BEFORE <name> <anchor> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FNode* anchor = FFIND(&e->flist, t[2]);
        if (anchor == nullptr) { cout << "anchor not found" << endl; return; }
        FPUSH_BEFORE(&e->flist, anchor, t[3]);
        cout << "-> " << t[3] << endl;
    }
    else if (cmd == "FDEL") {
        if (t.size() < 2) { cout << "usage: FDEL <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FDEL_FRONT(&e->flist);
        cout << "-> OK" << endl;
    }
    else if (cmd == "FDEL_BACK") {
        if (t.size() < 2) { cout << "usage: FDEL_BACK <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FDEL_BACK(&e->flist);
        cout << "-> OK" << endl;
    }
    // FDEL_AFTER <имя> <ориентир> - удалить узел после данного
    else if (cmd == "FDEL_AFTER") {
        if (t.size() < 3) { cout << "usage: FDEL_AFTER <name> <anchor>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FNode* anchor = FFIND(&e->flist, t[2]);
        if (anchor == nullptr) { cout << "anchor not found" << endl; return; }
        FDEL_AFTER(&e->flist, anchor);
        cout << "-> OK" << endl;
    }
    // FDEL_BEFORE <имя> <ориентир> - удалить узел перед данным
    else if (cmd == "FDEL_BEFORE") {
        if (t.size() < 3) { cout << "usage: FDEL_BEFORE <name> <anchor>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FNode* anchor = FFIND(&e->flist, t[2]);
        if (anchor == nullptr) { cout << "anchor not found" << endl; return; }
        FDEL_BEFORE(&e->flist, anchor);
        cout << "-> OK" << endl;
    }
    // FDEL_BY_VALUE <имя> <значение> - удалить узел по значению
    else if (cmd == "FDEL_BY_VALUE") {
        if (t.size() < 3) { cout << "usage: FDEL_BY_VALUE <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FDEL_BY_VALUE(&e->flist, t[2]);
        cout << "-> OK" << endl;
    }
    // FFIND <имя> <значение> - найти узел по значению
    else if (cmd == "FFIND") {
        if (t.size() < 3) { cout << "usage: FFIND <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FNode* f = FFIND(&e->flist, t[2]);
        cout << "-> " << (f ? "TRUE" : "FALSE") << endl;
    }
    else if (cmd == "FGET") {
        if (t.size() < 2) { cout << "usage: FGET <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FGET_HEAD(&e->flist);
    }
    else if (cmd == "FGET_TAIL") {
        if (t.size() < 2) { cout << "usage: FGET_TAIL <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FGET_TAIL(&e->flist);
    }
    else if (cmd == "FGET_REVERSE") {
        if (t.size() < 2) { cout << "usage: FGET_REVERSE <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_FLIST) { cout << "not found" << endl; return; }
        FGET_REVERSE(e->flist.head);
    }

    // ДВУСВЯЗНЫЙ СПИСОК
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
    // LPUSH_AFTER <имя> <ориентир> <значение>
    else if (cmd == "LPUSH_AFTER") {
        if (t.size() < 4) { cout << "usage: LPUSH_AFTER <name> <anchor> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        DNode* anchor = LFIND(&e->dlist, t[2]);
        if (anchor == nullptr) { cout << "anchor not found" << endl; return; }
        LPUSH_AFTER(&e->dlist, anchor, t[3]);
        cout << "-> " << t[3] << endl;
    }
    // LPUSH_BEFORE <имя> <ориентир> <значение>
    else if (cmd == "LPUSH_BEFORE") {
        if (t.size() < 4) { cout << "usage: LPUSH_BEFORE <name> <anchor> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        DNode* anchor = LFIND(&e->dlist, t[2]);
        if (anchor == nullptr) { cout << "anchor not found" << endl; return; }
        LPUSH_BEFORE(&e->dlist, anchor, t[3]);
        cout << "-> " << t[3] << endl;
    }
    else if (cmd == "LDEL") {
        if (t.size() < 2) { cout << "usage: LDEL <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LDEL_FRONT(&e->dlist);
        cout << "-> OK" << endl;
    }
    else if (cmd == "LDEL_BACK") {
        if (t.size() < 2) { cout << "usage: LDEL_BACK <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LDEL_BACK(&e->dlist);
        cout << "-> OK" << endl;
    }
        // LDEL_AFTER <имя> <ориентир> - удалить узел после данного
    else if (cmd == "LDEL_AFTER") {
        if (t.size() < 3) { cout << "usage: LDEL_AFTER <name> <anchor>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        DNode* anchor = LFIND(&e->dlist, t[2]);
        if (anchor == nullptr) { cout << "anchor not found" << endl; return; }
        LDEL_AFTER(&e->dlist, anchor);
        cout << "-> OK" << endl;
    }
    // LDEL_BEFORE <имя> <ориентир> - удалить узел перед данным
    else if (cmd == "LDEL_BEFORE") {
        if (t.size() < 3) { cout << "usage: LDEL_BEFORE <name> <anchor>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        DNode* anchor = LFIND(&e->dlist, t[2]);
        if (anchor == nullptr) { cout << "anchor not found" << endl; return; }
        LDEL_BEFORE(&e->dlist, anchor);
        cout << "-> OK" << endl;
    }
    // LDEL_BY_VALUE <имя> <значение>
    else if (cmd == "LDEL_BY_VALUE") {
        if (t.size() < 3) { cout << "usage: LDEL_BY_VALUE <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LDEL_BY_VALUE(&e->dlist, t[2]);
        cout << "-> OK" << endl;
    }
    else if (cmd == "LFIND") {
        if (t.size() < 3) { cout << "usage: LFIND <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        DNode* f = LFIND(&e->dlist, t[2]);
        cout << "-> " << (f ? "TRUE" : "FALSE") << endl;
    }
    else if (cmd == "LGET") {
        if (t.size() < 2) { cout << "usage: LGET <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LGET_HEAD(&e->dlist);
    }
    else if (cmd == "LGET_TAIL") {
        if (t.size() < 2) { cout << "usage: LGET_TAIL <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LGET_TAIL(&e->dlist);
    }
    else if (cmd == "LGET_REVERSE") {
        if (t.size() < 2) { cout << "usage: LGET_REVERSE <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DLIST) { cout << "not found" << endl; return; }
        LGET_REVERSE(&e->dlist);
    }

    // СТЕК 
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

    // ОЧЕРЕДЬ 
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

    // ДВУСВЯЗНАЯ ОЧЕРЕДЬ 
    else if (cmd == "DPUSH_BACK") {
        if (t.size() < 3) { cout << "usage: DPUSH_BACK <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        DPUSH_BACK(&e->deque, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "DPUSH_FRONT") {
        if (t.size() < 3) { cout << "usage: DPUSH_FRONT <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        DPUSH_FRONT(&e->deque, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "DPUSH") {
        if (t.size() < 3) { cout << "usage: DPUSH <name> <value>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        DPUSH_BACK(&e->deque, t[2]);
        cout << "-> " << t[2] << endl;
    }
    else if (cmd == "DPOP_FRONT") {
        if (t.size() < 2) { cout << "usage: DPOP_FRONT <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        cout << "-> " << DPOP_FRONT(&e->deque) << endl;
    }
    else if (cmd == "DPOP_BACK") {
        if (t.size() < 2) { cout << "usage: DPOP_BACK <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        cout << "-> " << DPOP_BACK(&e->deque) << endl;
    }
    else if (cmd == "DPOP") {
        if (t.size() < 2) { cout << "usage: DPOP <name>" << endl; return; }
        StructEntry* e = FindStruct(reg, t[1]);
        if (!e || e->type != TYPE_DEQUE) { cout << "not found" << endl; return; }
        cout << "-> " << DPOP_FRONT(&e->deque) << endl;
    }
    else if (cmd == "DGET" || cmd == "DGET_HEAD") {
        if (t.size() < 2) { cout << "usage: DGET <name>" << endl; return; }
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

    // ДЕРЕВО
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