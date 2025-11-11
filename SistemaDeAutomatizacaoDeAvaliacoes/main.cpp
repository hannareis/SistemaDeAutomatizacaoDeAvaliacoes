#include "MenuPrincipal.hpp"
#include <clocale>
#include <iostream>
#include "MenuFicha.hpp"
#include "MenuNotas.hpp"


using namespace std;

int main() {
    setlocale(LC_ALL, "Portuguese");

    cout << "\n=== Sistema de Automação de Avaliações ===\n";
    if (!autenticarOrganizador()) {
        cout << "\nAcesso negado. Encerrando o sistema...\n";
        return 0;
    }
    MenuNotas();
    menuPrincipal();
    MenuFicha();
    return 0;
}
