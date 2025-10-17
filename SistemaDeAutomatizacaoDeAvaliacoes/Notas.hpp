#ifndef NOTAS_HPP
#define NOTAS_HPP

#include <string>
using namespace std;

struct Notas {
    int id;
	double nota;
    string nome_avaliador;
    string equipe;
    string projeto;
};

void criarNotas();
void listarNotas();

,
#endif
