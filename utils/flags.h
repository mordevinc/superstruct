#ifndef FLAGS_H
#define FLAGS_H

#include <string>
#include <vector>

// один флаг: ключ и значение
struct Flag {
    std::string key;
    std::string value;
};

// все флаги командной строки
struct Flags {
    int count = 0;
    std::vector<Flag> items;
};

// обнулить набор флагов
void InitFlags(Flags* flags);

// добавить пару ключ-значение
void AddFlag(Flags* flags, const std::string& key, const std::string& value);

// получить значение по ключу, пустая строка если нет
std::string GetFlag(Flags* flags, const std::string& key);

// разобрать argc/argv из main
void ParseFlags(int argc, char* argv[], Flags* flags);

// печать всех флагов (отладка)
void PrintFlags(Flags* flags);

#endif