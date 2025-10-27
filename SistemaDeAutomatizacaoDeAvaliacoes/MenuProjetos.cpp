#include <iostream>
#include <limits>
#include "MenuProjetos.hpp"
#include "Projetos.hpp"
using namespace std;

void menuProjetos() {
    int opcao;
    do {
        cout << "\n=== Menu de Projetos ===\n";
        cout << "1. Criar Projeto\n";
        cout << "2. Listar Projetos\n";
        cout << "3. Atualizar Projeto\n";
        cout << "4. Deletar Projeto\n";
        cout << "0. Voltar\n";
        cout << "Escolha: ";
        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcao) {
        case 1: criarProjeto(); break;
        case 2: listarProjetos(); break;
        case 3: atualizarProjeto(); break;
        case 4: deletarProjeto(); break;
        case 0: cout << "Voltando ao menu principal...\n"; break;
        default: cout << "Opção inválida!\n";
        }
    } while (opcao != 0);
}
