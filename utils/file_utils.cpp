#include "file_utils.h"
#include <fstream>

// на разных системах mkdir называется по-разному
#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define MKDIR(path) mkdir(path, 0755)
#endif

// создаёт директорию, если её ещё нет
void EnsureDirectory(const std::string& path) {
    MKDIR(path.c_str());
}

// проверяет открывается ли файл на чтение
bool FileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

// если в имени файла нет слэшей - кладём его в папку data/
std::string BuildDataPath(const std::string& filename) {
    if (filename.find('/') != std::string::npos ||
        filename.find('\\') != std::string::npos) {
        return filename;
    }
    return "data/" + filename;
}