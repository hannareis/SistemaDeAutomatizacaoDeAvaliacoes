#include <iostream>
#include <limits>
#include "MenuNotas.hpp"
#include "Projetos.hpp"
#include "Notas.hpp"

using namespace std;

void MenuNotas() {
    int opcao;
    do {
        cout << "\n=== Menu de Notas ===\n";
        cout << "1. Criar Notas\n";
        cout << "2. Listar Notas\n";

        cout << "0. Voltar\n";
        cout << "Escolha: ";
        cin >> opcao;
       cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcao) {
        case 1: criarNotas(); break;
        case 2: listarNotas(); break;
        case 0: cout << "Voltando ao menu principal...\n"; break;
        default: cout << "Opção inválida!\n";
        }
    } while (opcao != 0);
}
