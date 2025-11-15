/*
===========================================================
Sistema de Automação de Avaliações
Módulo: Avaliadores
Autor: Thiago Carvalho Rodrigues
Descrição: Estruturas e funções do módulo responsável por
gerenciar avaliadores.
===========================================================
*/

#ifndef AVALIADOR_HPP
#define AVALIADOR_HPP

#include <string>
#include <vector>

struct Avaliador {
    std::string cpf;
    std::string nome;
    std::string email;
    std::string senha;
    std::string categoria;
    std::string areaEspecialidade;
};

// =============================
// CRUD + Login (Interface UI)
// =============================

/// Cria um avaliador interativamente (I/O em console).
void criarAvaliador();

/// Lista avaliadores em console.
void listarAvaliadores();

/// Atualiza campos de um avaliador (busca por CPF).
void atualizarAvaliador();

/// Deleta um avaliador (confirma em console).
void deletarAvaliador();

/// Fluxo de login (CPF + senha). Retorna true se autenticou.
[[nodiscard]] bool loginAvaliador();

// ===================================
// Auxiliares / Validação 
// ===================================

/// Autentica por CPF e senha. Lança exceções em erro.
[[nodiscard]] bool autenticarAvaliador(const std::string& cpf, const std::string& senha);

/// Verifica se o CPF já está cadastrado.
[[nodiscard]] bool verificarCPFExistente(const std::string& cpf);

/// Valida sintaxe e dígitos verificadores do CPF.
[[nodiscard]] bool validarCPF(const std::string& cpf);

/// Verifica duplicidade de e-mail.
[[nodiscard]] bool verificarEmailExistente(const std::string& email);

/// Remove pontuação do CPF.
[[nodiscard]] std::string limparCPF(const std::string& cpf);

// ===================================
// Persistência 
// ===================================

/// Carrega todos os avaliadores do armazenamento.
[[nodiscard]] std::vector<Avaliador> carregarAvaliadores();

/// Salva a lista completa (sobrescreve o arquivo).
void salvarAvaliadores(const std::vector<Avaliador>& lista);

#endif // AVALIADOR_HPP
