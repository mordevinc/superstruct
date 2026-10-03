#ifndef JSON_IO_H
#define JSON_IO_H

#include <string>
#include <vector>

// одна пара ключ-значение для нашего плоского json
struct JsonPair {
    std::string key;
    std::string value;
};

// весь json-объект: просто список пар
struct JsonObject {
    std::vector<JsonPair> pairs;
};

// создать пустой объект
JsonObject JsonCreate();

// положить ключ, если ключ уже есть - обновить значение
void JsonSet(JsonObject* obj, const std::string& key, const std::string& value);

// получить значение по ключу, если нет - пустая строка
std::string JsonGet(JsonObject* obj, const std::string& key);

// убрать ключ из объекта
void JsonRemove(JsonObject* obj, const std::string& key);

// превратить объект в текст
std::string JsonToString(JsonObject* obj);

// разобрать текст обратно в объект
JsonObject JsonParse(const std::string& text);

// запись и чтение файла
void JsonWriteToFile(JsonObject* obj, const std::string& filename);
JsonObject JsonReadFromFile(const std::string& filename);

#endif