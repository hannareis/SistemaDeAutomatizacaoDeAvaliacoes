#include <iostream>
#include <limits>
#include <string>
#include "MenuPrincipal.hpp"
#include "Menus.hpp"      // para menus de avaliadores e projetos
#include "Avaliador.hpp"  // se usar login de avaliador

using namespace std;

// === Login master (3 tentativas) ===
bool autenticarOrganizador() {
    const string USER = "admin";
    const string PASS = "admin123";

    for (int tent = 1; tent <= 3; ++tent) {
        string u, p;
        cout << "\n=== Login Organizador ===\n";
        cout << "Usuário: ";
        getline(cin, u);
        cout << "Senha : ";
        getline(cin, p);

        if (u == USER && p == PASS) {
            cout << " Acesso liberado!\n";
            return true;
        }
        cout << " Credenciais inválidas (" << tent << "/3). Tente novamente.\n";
    }
    cout << "Bloqueado: muitas tentativas.\n";
    return false;
}

// === Menu principal (sem pedir login de novo) ===
void menuPrincipal() {
    int opcao;
    do {
        cout << "\n=== Menu Principal ===\n";
        cout << "1. Gerenciar Avaliadores (Admin)\n";
        cout << "2. Gerenciar Projetos (Admin)\n";
        cout << "3. Banca de Notas (Avaliador)\n";
        cout << "0. Sair\n";
        cout << "Escolha: ";

        if (!(cin >> opcao)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opção inválida. Digite um número.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpa \n

        switch (opcao) {
        case 1: menuAvaliadores(); break;
        case 2: menuProjetos();    break;
        case 3: loginAvaliador();  break;
        case 0: cout << "\nEncerrando o sistema...\n"; break;
        default: cout << "Opção inválida. Tente novamente.\n";
        }

    } while (opcao != 0);
}
