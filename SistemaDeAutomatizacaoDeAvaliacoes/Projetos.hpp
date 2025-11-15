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

struct AvaliacaoRealizada {
    string avaliadorCpf;
	double nota;
};

struct Projeto {
    int id;
    string nome;
    string descricao;
    string responsavel;
    string categoria;
    string area;
    AvaliacaoRealizada avaliacao[3];
    double notaFinal = -1;
    string tipoAvaliacao;
};



// Funções principais (CRUD)

void criarProjeto();
void listarProjetos();
void atualizarProjeto();
void deletarProjeto();


//Funções auxiliares

vector<Projeto> carregarProjetos();
void salvarProjetos(const vector<Projeto>& lista);
bool projetoExiste(const string& nome);
string registrarAvaliacaoProjeto(int idProjeto, const string& cpf, double nota);
string atualizarAvaliacaoRealizada(int idProjeto, const string& cpf, double nota);
Projeto buscarProjeto(const string& nome);
bool projetoFinalizado(const Projeto& p);
int contarAvaliacoes(const Projeto& p);

#endif
