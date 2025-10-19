#include <iostream>
#include "Projetos.hpp"
#include "Avaliador.hpp"
using namespace std;

int main() {
    int opcao;
    do {
        cout << "\n=== Sistema de Automacao de Avaliacoes ===\n";
        cout << "1. Gerenciar Projetos\n";
        cout << "2. Gerenciar Avaliadores\n";
        cout << "0. Sair\n";
        cout << "Escolha: ";
        cin >> opcao;
        cin.ignore();

        switch (opcao) {
        case 1: {
            int subOpcao;
            do {
                cout << "\n=== Sistema de Gerenciamento de Projetos ===\n";
                cout << "1. Criar projeto\n";
                cout << "2. Listar projetos\n";
                cout << "3. Atualizar projeto\n";
                cout << "4. Deletar projeto\n";
                cout << "0. Voltar\n";
                cout << "Escolha: ";
                cin >> subOpcao;
                cin.ignore();

                switch (subOpcao) {
                case 1: criarProjeto(); break;
                case 2: listarProjetos(); break;
                case 3: atualizarProjeto(); break;
                case 4: deletarProjeto(); break;
                case 0: break;
                default: cout << "Opcao invalida.\n"; break;
                }
            } while (subOpcao != 0);
            break;
        }
        case 2: {
            int subOpcao;
            do {
                cout << "\n=== Sistema de Gerenciamento de Avaliadores ===\n";
                cout << "1. Criar avaliador\n";
                cout << "2. Listar avaliadores\n";
                cout << "3. Atualizar avaliador\n";
                cout << "4. Deletar avaliador\n";
                cout << "0. Voltar\n";
                cout << "Escolha: ";
                cin >> subOpcao;
                cin.ignore();

                switch (subOpcao) {
                case 1: criarAvaliador(); break;
                case 2: listarAvaliadores(); break;
                case 3: atualizarAvaliador(); break;
                case 4: deletarAvaliador(); break;
                case 0: break;
                default: cout << "Opcao invalida.\n"; break;
                }
            } while (subOpcao != 0);
            break;
        }
        case 0:
            cout << "Saindo...\n";
            break;
        default:
            cout << "Opcao invalida.\n";
            break;
        }
    } while (opcao != 0);

    return 0;
}