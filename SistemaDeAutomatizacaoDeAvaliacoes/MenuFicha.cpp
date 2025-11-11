#include "Ficha.hpp"
#include <iostream>
#include "MenuFicha.hpp"
using namespace std;

int MenuFicha() {
    SistemaFichas sistema;
    inicializarSistema(sistema);

    carregarFichas(sistema); // 🔹 Carrega as fichas salvas

    int opcao;
    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Criar nova ficha\n";
        cout << "2. Listar fichas\n";
        cout << "0. Sair\n";
        cout << "Escolha: ";
        cin >> opcao;
        cin.ignore();

        switch (opcao) {
        case 1:
            criarFicha(sistema);
            salvarFichas(sistema); // 🔹 Salva após criar
            break;
        case 2:
            listarFichas(sistema);
            break;
        case 0:
            cout << "\nEncerrando e salvando dados...\n";
            salvarFichas(sistema);
            break;
        default:
            cout << "\nOpcao invalida!\n";
        }
    } while (opcao != 0);

    liberarSistema(sistema);
    return 0;
}
