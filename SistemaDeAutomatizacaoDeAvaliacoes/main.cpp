#include "MenuPrincipal.hpp"
#include <clocale>
#include <iostream>
#include "MenuFicha.hpp"
#include "MenuNotas.hpp"


using namespace std;

int main() {
    setlocale(LC_ALL, "Portuguese");

    cout << "\n=== Sistema de Automação de Avaliações ===\n";
	Token token = autenticarUser();
    if (!token.sucesso) {
        cout << "\nAcesso negado. Encerrando o sistema...\n";
        return 0;
    }
    else {
        if (token.tipo=="admin") {
			cout << "\nAcesso concedido. Bem-vindo ao sistema!\n";
            menuPrincipal();
        }
    }

    MenuNotas();
   
    MenuFicha();
    return 0;
}
