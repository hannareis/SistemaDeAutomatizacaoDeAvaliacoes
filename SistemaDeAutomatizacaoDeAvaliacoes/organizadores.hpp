#ifndef ORGANIZADORES_HPP
#define ORGANIZADORES_HPP

#include <string>
using namespace std;

struct Organizador {
    int id;
    string nome_organizador;
    string area_organizador;
};

void cadastrarOrganizador();
void editarOrganizador();
void excluirOrganizador();
void buscarOrganizador();

#endif
