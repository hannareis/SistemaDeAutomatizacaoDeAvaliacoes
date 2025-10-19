#ifndef AVALIADOR_HPP
#define AVALIADOR_HPP

#include <string>
using namespace std;

struct Avaliador {
    int id;
    string nome;
    string email;
    string categoria;
    string area_especialidade;
};

void criarAvaliador();
void listarAvaliadores();
void atualizarAvaliador();
void deletarAvaliador();

#endif