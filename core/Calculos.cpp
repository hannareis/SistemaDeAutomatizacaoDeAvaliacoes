#include <vector>

// Função recursiva que soma todos os valores de um vetor
double somaRecursiva(const std::vector<double>& v, size_t i) {
    if (i >= v.size()) return 0.0;
    return v[i] + somaRecursiva(v, i + 1);
}

// Função que calcula a média usando a soma recursiva
double mediaRecursiva(const std::vector<double>& v) {
    if (v.empty()) return 0.0;
    return somaRecursiva(v, 0) / (double)v.size();
}
