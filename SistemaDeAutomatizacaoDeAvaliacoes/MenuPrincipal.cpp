#include <iostream>
#include "MenuPrincipal.hpp"
#include "MenuAvaliadores.hpp"
#include "MenuProjetos.hpp"
#include "Avaliador.hpp"  // para usar loginAvaliador()

using namespace std;

void menuPrincipal() {
    int opcao;
    do {
        cout << "\n=== Sistema de Automação de Avaliações ===\n";
        cout << "1. Gerenciar Avaliadores (Admin)\n";
        cout << "2. Gerenciar Projetos (Admin)\n";
        cout << "3. Login do Avaliador\n";
        cout << "0. Sair\n";
        cout << "Escolha: ";
        cin >> opcao;
        cin.ignore();

        switch (opcao) {
        case 1:
            menuAvaliadores();
            break;
        case 2:
            menuProjetos();
            break;
        case 3:
            loginAvaliador();  // função do Avaliador.cpp
            break;
        case 0:
            cout << "Encerrando o sistema...\n";
            break;
        default:
            cout << "Opção inválida. Tente novamente.\n";
        }
    } while (opcao != 0);
}
