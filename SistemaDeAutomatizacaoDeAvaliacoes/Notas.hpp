#ifndef NOTAS_HPP
#define NOTAS_HPP

#include <string>
using namespace std;

struct Notas {
    int id = 0;
    double nota = 0.0;
	double notasCriterio[5] = { 0.0 };
    string nome_avaliador;
    string projeto;
};

void criarNotas();
void listarNotas();

#endif
