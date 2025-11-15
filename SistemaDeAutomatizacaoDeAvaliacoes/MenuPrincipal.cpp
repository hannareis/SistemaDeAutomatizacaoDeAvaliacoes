#include <iostream>
#include <limits>
#include <string>
#include <exception>

#include "MenuPrincipal.hpp"
#include "Menus.hpp"      // menuAvaliadores, menuProjetos
#include "Avaliador.hpp"  // autenticarAvaliador
#include "MenuNotas.hpp"  // MenuNotas
#include "MenuFicha.hpp"  // MenuFicha

using namespace std;

namespace {

    // ===== Área do ADMIN (cadastra avaliador e projeto) =====
    void menuAreaAdmin() {
        int opcao;
        do {
            cout << "\n=== Área do Administrador ===\n";
            cout << "1. Gerenciar Avaliadores\n";
            cout << "2. Gerenciar Projetos\n";
            cout << "0. Logout\n";
            cout << "Escolha: ";

            if (!(cin >> opcao)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Opção inválida. Digite um número.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (opcao) {
            case 1:
                menuAvaliadores();
                break;
            case 2:
                menuProjetos();
                break;
            case 0:
                cout << "Saindo da área do administrador...\n";
                break;
            default:
                cout << "Opção inválida. Tente novamente.\n";
            }
        } while (opcao != 0);
    }

    // ===== Área do AVALIADOR (fichas + notas) =====
    void menuAreaAvaliador() {
        int opcao;
        do {
            cout << "\n=== Área do Avaliador ===\n";
            cout << "1. Gerenciar Fichas de Avaliação\n";
            cout << "2. Banca de Notas\n";
            cout << "0. Logout\n";
            cout << "Escolha: ";

            if (!(cin >> opcao)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Opção inválida. Digite um número.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (opcao) {
            case 1:
                MenuFicha();
                break;
            case 2:
                MenuNotas();
                break;
            case 0:
                cout << "Saindo da área do avaliador...\n";
                break;
            default:
                cout << "Opção inválida. Tente novamente.\n";
            }
        } while (opcao != 0);
    }

} // namespace interno

// ===== Tela de login única =====
void menuPrincipal() {
    while (true) {
        cout << "\n=== Tela de Login ===\n";
        cout << "(Digite 0 no usuário para sair)\n";
        cout << "Usuário (admin ou CPF): ";
        string usuario;
        getline(cin, usuario);

        if (usuario == "0") {
            cout << "\nEncerrando o sistema...\n";
            return;
        }

        cout << "Senha: ";
        string senha;
        getline(cin, senha);

        // ---- 1) Tenta login como ADMIN ----
        if (usuario == "admin") {
            if (senha == "admin123") {
                cout << "\n  Login de administrador realizado com sucesso!\n";
                menuAreaAdmin();     // entra no menu do admin
                continue;            // ao dar logout volta pra tela de login
            }
            else {
                cout << "Senha de administrador incorreta.\n";
                continue;
            }
        }

        // ---- 2) Tenta login como AVALIADOR (CPF + senha) ----
        try {
            if (autenticarAvaliador(usuario, senha)) {
                cout << "\n Login de avaliador realizado com sucesso!\n";
                menuAreaAvaliador(); // entra na área do avaliador
                // ao sair de lá, volta pra tela de login
            }
        }
        catch (const std::exception& e) {
            cout << "[Erro de Login] " << e.what() << "\n";
        }
    }
}
