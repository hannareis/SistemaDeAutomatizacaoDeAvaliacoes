#include <iostream>
#include "Projetos.hpp"
#include "Projetos.hpp"
using namespace std;

int main() {
    int opcao;
    do {
        cout << "\n=== Sistema de Gerenciamento de Projetos ===\n";
        cout << "1. Criar projeto\n";
        cout << "2. Listar projetos\n";
        cout << "3. Atualizar projeto\n";
        cout << "4. Deletar projeto\n";
        cout << "0. Sair\n";
        cout << "Escolha: ";
        cin >> opcao; cin.ignore();

        switch (opcao) {
        case 1: criarProjeto(); break;
        case 2: listarProjetos(); break;
        case 3: atualizarProjeto(); break;
        case 4: deletarProjeto(); break;
        case 0: cout << "Saindo...\n"; break;
        default: cout << "Opcao invalida.\n"; break;
        }
    } while (opcao != 0);

    return 0;
}
