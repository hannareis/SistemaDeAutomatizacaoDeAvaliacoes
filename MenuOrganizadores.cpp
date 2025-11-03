#include <iostream>
#include <limits>
#include "organizadores.hpp"
#include "organizadores.hpp"
#include "TratamentoExcecao.hpp"
using namespace std;


void menuOrganizadores() {
	int opcao;
	do {
		cout << "\n=== Menu de Organizadores ===\n";
		cout << "1. Cadastrar Organizador\n";
		cout << "2. Listar Organizadores\n";
		cout << "3. Atualizar Organizador\n";
		cout << "4. Deletar Organizador\n";
		cout << "0. Voltar\n";
		cout << "Escolha: ";
		cin >> opcao;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		switch (opcao) {
		case 1: criarOrganizador();; break;
		case 2: listarOrganizadores(); break;
		case 3: atualizarOrganizador(); break;
		case 4: deletarOrganizador(); break;
		case 0: cout << "Voltando ao menu principal...\n"; break;
		default: cout << "Opção inválida!\n";
		}
	} while (opcao != 0);
}