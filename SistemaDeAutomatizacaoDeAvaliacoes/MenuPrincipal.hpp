#ifndef MENU_PRINCIPAL_HPP
#define MENU_PRINCIPAL_HPP

#include <string>

struct Token {
	bool sucesso;
	std::string tipo; // "admin", "cpf", "bloqueado"
};

// Login master do admin
Token autenticarUser();

// Menu principal do sistema
void menuPrincipal();

#endif
