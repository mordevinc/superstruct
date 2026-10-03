#include "json_io.h"
#include <fstream>
#include <sstream>
#include <iostream>

// пустой объект
JsonObject JsonCreate() {
    JsonObject obj;
    obj.pairs.clear();
    return obj;
}

// если ключ уже есть - обновляем, иначе добавляем
void JsonSet(JsonObject* obj, const std::string& key, const std::string& value) {
    for (int i = 0; i < (int)obj->pairs.size(); i++) {
        if (obj->pairs[i].key == key) {
            obj->pairs[i].value = value;
            return;
        }
    }
    JsonPair p;
    p.key = key;
    p.value = value;
    obj->pairs.push_back(p);
}

// поиск значения по ключу
std::string JsonGet(JsonObject* obj, const std::string& key) {
    for (int i = 0; i < (int)obj->pairs.size(); i++) {
        if (obj->pairs[i].key == key) return obj->pairs[i].value;
    }
    return "";
}

// удаление ключа
void JsonRemove(JsonObject* obj, const std::string& key) {
    for (int i = 0; i < (int)obj->pairs.size(); i++) {
        if (obj->pairs[i].key == key) {
            // erase сдвигает элементы после i на одну позицию влево
            obj->pairs.erase(obj->pairs.begin() + i);
            return;
        }
    }
}

// сериализация: каждая пара на отдельной строке, без запятой в конце
std::string JsonToString(JsonObject* obj) {
    std::string result = "{\n";
    for (int i = 0; i < (int)obj->pairs.size(); i++) {
        result += "  \"" + obj->pairs[i].key + "\": \"" + obj->pairs[i].value + "\"";
        if (i != (int)obj->pairs.size() - 1) result += ",";
        result += "\n";
    }
    result += "}";
    return result;
}

// разбор json: ключи и значения всегда в кавычках
JsonObject JsonParse(const std::string& text) {
    JsonObject obj = JsonCreate();
    std::string key, value;

    // inString - между кавычками ли мы сейчас
    // isKey - читаем ключ
    // isValue - читаем значение
    // firstQuoteDone - помогает отличить первую кавычку (начало ключа) от второй (начало значения)
    bool inString = false;
    bool isKey = false;
    bool isValue = false;
    bool firstQuoteDone = false;

    for (int i = 0; i < (int)text.size(); i++) {
        char c = text[i];

        if (c == '"') {
            inString = !inString;
            if (inString) {
                // открылась кавычка: если первая после { или , - это ключ
                if (!firstQuoteDone) {
                    isKey = true;
                    isValue = false;
                    firstQuoteDone = true;
                } else {
                    isKey = false;
                    isValue = true;
                }
            } else {
                // закрылась кавычка: если закрывали значение - сохраняем пару
                if (isValue) {
                    JsonSet(&obj, key, value);
                    key = "";
                    value = "";
                    isKey = false;
                    isValue = false;
                    firstQuoteDone = false;
                }
            }
            continue;
        }

        // копим символы только внутри кавычек
        if (inString) {
            if (isKey) key += c;
            else if (isValue) value += c;
        }
    }

    return obj;
}

// запись в файл
void JsonWriteToFile(JsonObject* obj, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Cannot open file: " << filename << std::endl;
        return;
    }
    file << JsonToString(obj);
    file.close();
}

// чтение всего файла и разбор его как json
JsonObject JsonReadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Cannot open file: " << filename << std::endl;
        return JsonCreate();
    }
    // rdbuf() - читает весь файл разом в буфер
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return JsonParse(buffer.str());
}