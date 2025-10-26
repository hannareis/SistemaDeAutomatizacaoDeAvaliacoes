#include "Paths.h"
#include <filesystem>

std::string dataDir() {
    namespace fs = std::filesystem;
    // Se o programa foi executado a partir da RAIZ do projeto
    if (fs::exists("data")) return "data";
    // Se o programa foi executado a partir de build/
    if (fs::exists("../data")) return "../data";
    // Fallback (mantém "data" por padrão)
    return "data";
}
