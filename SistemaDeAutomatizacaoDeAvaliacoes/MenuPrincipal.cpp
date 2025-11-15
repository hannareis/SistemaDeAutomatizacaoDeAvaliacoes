#include <iostream>
#include <limits>
#include <string>
#include "MenuPrincipal.hpp"
#include "Menus.hpp"      // para menus de avaliadores e projetos
#include "Avaliador.hpp"  // se usar login de avaliador
#include "MenuNotas.hpp"
#include "MenuFicha.hpp"



using namespace std;


using namespace std;

// Credenciais master
const string USER = "admin";
const string PASS = "admin123";

// Funções auxiliares simuladas (substitua pelas reais do seu projeto)
bool validarCPFAuth(const string& cpf) {
    // Verifica se tem 11 dígitos numéricos
	return validarCPF(cpf);
}

bool verificarCPFExistenteAuth(const string& cpf) {
    // Aqui você pode consultar no arquivo ou vetor real
    // Exemplo temporário: só aceita um CPF de teste
    return verificarCPFExistente(cpf);
}

// ======================================================
// Função: identificarTipoUsuario
// Retorna "admin", "cpf" ou "desconhecido"
// ======================================================
string identificarTipoUsuario(const string& entrada) {
    if (entrada == USER) {
        return "admin";
    }
    else if (validarCPF(entrada)) {
        if (verificarCPFExistente(entrada)) {
            return "cpf";
        }
        else {
            cout << "❌ CPF não encontrado.\n";
            return "nao_encontrado";
        }
    }
    else {
        return "desconhecido";
    }
}



// ======================================================
// Função: autenticar
// Realiza login com até 3 tentativas
// ======================================================
Token autenticarUser() {
    for (int tent = 1; tent <= 3; ++tent) {
        string u, p;
        cout << "\n=== Login ===\n";
        cout << "Usuário (CPF ou admin): ";
        getline(cin, u);
        cout << "Senha : ";
        getline(cin, p);

        string tipo = identificarTipoUsuario(u); // "admin", "cpf", "nao_encontrado", etc.

        // ADMIN
        if (tipo == "admin") {
            if (p == PASS) {
                cout << "✅ Acesso liberado (Administrador)\n";
                return Token{ true, "admin" };
            }
            else {
                cout << "❌ Senha incorreta para administrador.\n";
            }
        }

        // CPF
        else if (tipo == "cpf") {
            bool senhaOK = verificarSenhaExistente(u);

            if (senhaOK) {
                cout << "✅ Login via CPF reconhecido.\n";
                return Token{ true, "cpf" };
            }
            else {
                cout << "❌ Senha incorreta para CPF informado.\n";
            }
        }

        // NÃO ENCONTRADO
        else if (tipo == "nao_encontrado") {
            cout << "⚠️ CPF não registrado no sistema.\n";
        }

        // FORMATO INVALIDO
        else {
            cout << "→ Formato inválido de identificador.\n";
        }

        cout << "Tentativa " << tent << "/3\n";
    }

    cout << "🔒 Bloqueado: muitas tentativas.\n";
    return Token{ false, "bloqueado" };
}



// === Menu principal (sem pedir login de novo) ===
void menuPrincipal() {
    int opcao;
    do {
        cout << "\n=== Menu Principal ===\n";
        cout << "1. Gerenciar Avaliadores (Admin)\n";
        cout << "2. Gerenciar Projetos (Admin)\n";
        cout << "3. Ficha de Avaliação (Admin)\n";
        cout << "4. Banca de Notas (Avaliador)\n";
        cout << "5. Avaliar Projetos (Avaliador)\n";
		
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
        case 3: MenuFicha();       break;
        case 4: loginAvaliador();  break;
		case 5: MenuNotas();       break;
        case 0: cout << "\nEncerrando o sistema...\n"; break;
        default: cout << "Opção inválida. Tente novamente.\n";
        }

    } while (opcao != 0);
}
