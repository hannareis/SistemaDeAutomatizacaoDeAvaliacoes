#ifndef PROJETO_HPP
#define PROJETO_HPP

#include <string>
using namespace std;

struct Projeto {
    int id;
    string nome;
    string descricao;
    string responsavel;
};

void criarProjeto();
void listarProjetos();
void atualizarProjeto();
void deletarProjeto();

#endif
