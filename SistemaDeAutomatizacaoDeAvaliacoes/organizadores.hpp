#ifndef ORGANIZADORES_HPP
#define ORGANIZADORES_HPP

#include <string>
using namespace std;

struct Organizador {
    int id;
    string nome;
    string area;
};

// Funções CRUD
void criarOrganizador();
void listarOrganizadores();
void atualizarOrganizador();
void deletarOrganizador();

// Funções de busca
void buscarPorID();
void buscarPorNome();
void buscarPorArea();

#endif
