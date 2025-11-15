#ifndef NOTAS_HPP
#define NOTAS_HPP
#include <ctime>
#include <string>
#include "Ficha.hpp"
using namespace std;

struct Notas {
    string data;
    double nota = 0.0;
	string tipo_avaliacao;
    string nome_avaliador;
    string projeto;
};

void criarNotas();
void listarNotas();
double registrarNotas(SistemaFichas& s, const string& tipoAvaliacao);
struct NotaQuesito {
    double nota;
	double peso; 
};

#endif
