/*
===========================================================
Sistema de Automação de Avaliações
Módulo: Tratamento de Exceções
Autor: Thiago Carvalho Rodrigues
Descrição: Define as classes de exceção personalizadas
para validações e erros do sistema.
===========================================================
*/

#ifndef TRATAMENTO_EXCECAO_HPP
#define TRATAMENTO_EXCECAO_HPP

#include <string>
#include <exception>
using namespace std;

// Falhas de autenticação (login, CPF duplicado, etc.)

class ExcecaoAutenticacao : public exception {
private:
    string mensagem;
public:
    explicit ExcecaoAutenticacao(const string& msg) : mensagem(msg) {}
    const char* what() const noexcept override { return mensagem.c_str(); }
};

//CPF inválido

class ExcecaoCPFInvalido : public exception {
private:
    string mensagem;
public:
    explicit ExcecaoCPFInvalido(const string& msg) : mensagem(msg) {}
    const char* what() const noexcept override { return mensagem.c_str(); }
};

// Senha incorreta

class ExcecaoSenhaIncorreta : public exception {
private:
    string mensagem;
public:
    explicit ExcecaoSenhaIncorreta(const string& msg) : mensagem(msg) {}
    const char* what() const noexcept override { return mensagem.c_str(); }
};

//Campo obrigatório não preenchido

class ExcecaoCampoVazio : public exception {
private:
    string mensagem;
public:
    explicit ExcecaoCampoVazio(const string& msg) : mensagem(msg) {}
    const char* what() const noexcept override { return mensagem.c_str(); }
};

#endif
