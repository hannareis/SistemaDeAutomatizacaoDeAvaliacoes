/*
===========================================================
Sistema de Automação de Avaliações
Módulo: Projetos
Autor: Thiago Carvalho Rodrigues
Descrição: Define a estrutura e as funções responsáveis
por gerenciar o cadastro, atualização, exclusão e vínculo
de projetos com avaliadores da mesma área e categoria.
===========================================================
*/

#ifndef PROJETOS_HPP
#define PROJETOS_HPP

#include <string>
#include <vector>
using namespace std;


struct Projeto {
    int id;
    string nome;
    string descricao;
    string responsavel;
    string categoria;
    string area;
    vector<string> avaliadoresCPF; // exatamente 3 avaliadores
};


// Funções principais (CRUD)

void criarProjeto();
void listarProjetos();
void atualizarProjeto();
void deletarProjeto();


//Funções auxiliares

vector<Projeto> carregarProjetos();
void salvarProjetos(const vector<Projeto>& lista);

#endif
