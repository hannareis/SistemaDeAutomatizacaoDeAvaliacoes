/*
===========================================================
Sistema de Automação de Avaliações
Módulo: Avaliadores
Autor: Thiago Carvalho Rodrigues
Descrição: Define a estrutura e as funções do módulo
responsável por gerenciar os avaliadores do sistema.
===========================================================
*/

#ifndef AVALIADOR_HPP
#define AVALIADOR_HPP

#include <string>
#include <vector>
using namespace std;


struct Avaliador {
    string cpf;
    string nome;
    string email;
    string senha;
    string categoria;
    string areaEspecialidade;
};


//Funções principais (CRUD + login)

void criarAvaliador();
void listarAvaliadores();
void atualizarAvaliador();
void deletarAvaliador();
void loginAvaliador();
void menuCategoriaEArea(string& categoria, string& areaEspecialidade);



//Funções auxiliares e de validação

bool autenticarAvaliador(const string& cpf, const string& senha);
bool verificarCPFExistente(const string& cpf);
bool validarCPF(const string& cpf);
bool verificarEmailExistente(const string& email);
string limparCPF(const string& cpf);            
vector<Avaliador> carregarAvaliadores();        
void salvarAvaliadores(const vector<Avaliador>& lista); 

#endif
