#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

// создать директорию, если её нет
void EnsureDirectory(const std::string& path);

// проверить, существует ли файл
bool FileExists(const std::string& path);

// если путь относительный - подставить префикс data/
std::string BuildDataPath(const std::string& filename);

#endif