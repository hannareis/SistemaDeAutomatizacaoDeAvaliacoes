#include "MenuPrincipal.hpp"
#include <clocale>
#include <iostream>

using namespace std;

int main() {
    setlocale(LC_ALL, "Portuguese");

    cout << "\n=== Sistema de Automação de Avaliações ===\n";

    // Já faz o login único e redireciona para admin ou avaliador
    menuPrincipal();

    return 0;
}

