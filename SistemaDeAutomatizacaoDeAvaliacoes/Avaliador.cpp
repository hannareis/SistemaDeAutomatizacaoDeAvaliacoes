/*
===========================================================
Sistema de Automação de Avaliações
Módulo: Avaliadores
Autor: Thiago Carvalho Rodrigues
Descrição: Gerencia cadastro, validação, atualização,
autenticação e exclusão de avaliadores.
===========================================================
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include "Avaliador.hpp"
#include "TratamentoExcecao.hpp"
#include "MenuAvaliadores.hpp"

using namespace std;

const string ARQUIVO_AVALIADOR = "avaliadores.txt";

//Validação de CPF (aceita com pontos e traço)

bool validarCPF(const string& cpf) {
    string apenasDigitos;
    for (char c : cpf)
        if (isdigit(c)) apenasDigitos += c;

    if (apenasDigitos.length() != 11) return false;
    if (apenasDigitos == string(11, apenasDigitos[0])) return false;

    int soma = 0, resto;
    for (int i = 0; i < 9; i++)
        soma += (apenasDigitos[i] - '0') * (10 - i);
    resto = (soma * 10) % 11;
    if (resto == 10) resto = 0;
    if (resto != (apenasDigitos[9] - '0')) return false;

    soma = 0;
    for (int i = 0; i < 10; i++)
        soma += (apenasDigitos[i] - '0') * (11 - i);
    resto = (soma * 10) % 11;
    if (resto == 10) resto = 0;
    if (resto != (apenasDigitos[10] - '0')) return false;

    return true;
}

//Normaliza CPF (remove pontos e traço)

string limparCPF(const string& cpf) {
    string apenasDigitos;
    for (char c : cpf)
        if (isdigit(c)) apenasDigitos += c;
    return apenasDigitos;
}


//Carrega avaliadores do arquivo

vector<Avaliador> carregarAvaliadores() {
    vector<Avaliador> lista;
    ifstream file(ARQUIVO_AVALIADOR);
    if (!file.is_open()) return lista;

    string linha;
    while (getline(file, linha)) {
        if (linha.empty()) continue;
        Avaliador a;
        stringstream ss(linha);
        getline(ss, a.cpf, ';');
        getline(ss, a.nome, ';');
        getline(ss, a.email, ';');
        getline(ss, a.senha, ';');
        getline(ss, a.categoria, ';');
        getline(ss, a.areaEspecialidade, ';');
        lista.push_back(a);
    }
    return lista;
}


//  Salva avaliadores no arquivo

void salvarAvaliadores(const vector<Avaliador>& lista) {
    ofstream file(ARQUIVO_AVALIADOR, ios::trunc);
    for (const auto& a : lista)
        file << limparCPF(a.cpf) << ";" << a.nome << ";" << a.email << ";" << a.senha
        << ";" << a.categoria << ";" << a.areaEspecialidade << "\n";
}


//  Verifica CPF duplicado (recursivo, padronizado)

bool verificarRecursivo(const vector<Avaliador>& lista, const string& cpf, int i) {
    if (i >= (int)lista.size()) return false;

    string cpfLista = limparCPF(lista[i].cpf);
    string cpfDigitado = limparCPF(cpf);
    if (cpfLista == cpfDigitado) return true;

    return verificarRecursivo(lista, cpf, i + 1);
}

bool verificarCPFExistente(const string& cpf) {
    auto lista = carregarAvaliadores();
    return verificarRecursivo(lista, cpf, 0);
}


//  Verifica se o e-mail já existe (evita duplicação)

bool verificarEmailExistente(const string& email) {
    auto lista = carregarAvaliadores();
    for (const auto& a : lista)
        if (a.email == email) return true;
    return false;
}


//  Criação de avaliador

void criarAvaliador() {
    Avaliador a;
    cout << "\n=== Cadastro de Avaliador ===\n";

    cout << "Nome: ";
    getline(cin, a.nome);
    if (a.nome.empty()) throw ExcecaoCampoVazio("O nome não pode estar em branco.");

    // Valida e-mail
    while (true) {
        cout << "E-mail: ";
        getline(cin, a.email);
        try {
            if (a.email.empty())
                throw ExcecaoCampoVazio("O e-mail não pode estar em branco.");

            size_t posArroba = a.email.find('@');
            size_t posPonto = a.email.find('.', posArroba);
            if (posArroba == string::npos || posPonto == string::npos || posPonto <= posArroba + 1)
                throw ExcecaoAutenticacao("Formato de e-mail inválido! Use algo como nome@dominio.com.");

            if (verificarEmailExistente(a.email))
                throw ExcecaoAutenticacao("Este e-mail já está cadastrado.");

            break;
        }
        catch (const exception& e) {
            cout << "[Erro de E-mail] " << e.what() << "\nTente novamente.\n";
        }
    }

    // Valida CPF
    while (true) {
        cout << "CPF (com ou sem pontos): ";
        getline(cin, a.cpf);
        try {
            if (!validarCPF(a.cpf))
                throw ExcecaoCPFInvalido("CPF inválido!");
            if (verificarCPFExistente(a.cpf))
                throw ExcecaoAutenticacao("Este CPF já está cadastrado.");
            break;
        }
        catch (const exception& e) {
            cout << "[Erro de CPF] " << e.what() << "\nTente novamente.\n";
        }
    }

    // Senha
    cout << "Senha: ";
    getline(cin, a.senha);
    if (a.senha.empty()) throw ExcecaoCampoVazio("A senha não pode estar vazia.");

    // Categoria e área
    menuCategoriaEArea(a.categoria, a.areaEspecialidade);

    // Salva
    ofstream file(ARQUIVO_AVALIADOR, ios::app);
    file << limparCPF(a.cpf) << ";" << a.nome << ";" << a.email << ";" << a.senha
        << ";" << a.categoria << ";" << a.areaEspecialidade << "\n";
    file.close();

    cout << "\n Avaliador cadastrado com sucesso!\n";
}


//  Listagem

void listarAvaliadores() {
    auto lista = carregarAvaliadores();
    if (lista.empty()) {
        cout << "\nNenhum avaliador cadastrado.\n";
        return;
    }

    cout << "\n=== Lista de Avaliadores ===\n";
    for (const auto& a : lista) {
        cout << "\nCPF: " << a.cpf
            << "\nNome: " << a.nome
            << "\nE-mail: " << a.email
            << "\nCategoria: " << a.categoria
            << "\nÁrea: " << a.areaEspecialidade << "\n";
    }
}


//  Atualização de Avaliador (menu interativo)

void atualizarAvaliador() {
    auto lista = carregarAvaliadores();
    string cpf;
    cout << "\n=== Atualizar Avaliador ===\nCPF: ";
    getline(cin, cpf);

    string cpfDigitado = limparCPF(cpf);
    bool achou = false;

    for (auto& a : lista) {
        if (limparCPF(a.cpf) == cpfDigitado) {
            achou = true;
            int opcao;

            do {
                cout << "\nO que deseja atualizar?\n";
                cout << "1. E-mail\n2. Senha\n3. Categoria e Área\n0. Voltar\nEscolha: ";
                cin >> opcao;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (opcao) {
                case 1: {
                    string novoEmail;
                    cout << "Novo e-mail: ";
                    getline(cin, novoEmail);

                    if (verificarEmailExistente(novoEmail))
                        cout << "E-mail já cadastrado!\n";
                    else if (!novoEmail.empty()) {
                        a.email = novoEmail;
                        cout << "E-mail atualizado!\n";
                    }
                    break;
                }
                case 2: {
                    string novaSenha;
                    cout << "Nova senha: ";
                    getline(cin, novaSenha);
                    if (!novaSenha.empty()) {
                        a.senha = novaSenha;
                        cout << "Senha atualizada!\n";
                    }
                    break;
                }
                case 3:
                    menuCategoriaEArea(a.categoria, a.areaEspecialidade);
                    cout << "Categoria e área atualizadas!\n";
                    break;
                case 0:
                    cout << "Voltando...\n";
                    break;
                default:
                    cout << "Opção inválida.\n";
                }
            } while (opcao != 0);
            break;
        }
    }

    if (achou) {
        salvarAvaliadores(lista);
        cout << "\n Avaliador atualizado com sucesso!\n";
    }
    else {
        cout << "\n CPF não encontrado.\n";
    }
}


//  Exclusão com Confirmação

void deletarAvaliador() {
    auto lista = carregarAvaliadores();
    string cpf;
    cout << "\n=== Deletar Avaliador ===\nCPF: ";
    getline(cin, cpf);

    string cpfDigitado = limparCPF(cpf);
    vector<Avaliador> novaLista;
    Avaliador alvo;
    bool achou = false;

    for (auto& a : lista) {
        if (limparCPF(a.cpf) == cpfDigitado) {
            achou = true;
            alvo = a;
        }
        else {
            novaLista.push_back(a);
        }
    }

    if (!achou) {
        cout << "\n CPF não encontrado.\n";
        return;
    }

    cout << "\nAvaliador encontrado:\n";
    cout << "Nome: " << alvo.nome << "\nÁrea: " << alvo.areaEspecialidade << "\n";
    cout << "\nTem certeza que deseja excluir?\n1. Sim\n2. Não\nEscolha: ";

    int op;
    cin >> op;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (op == 1) {
        salvarAvaliadores(novaLista);
        cout << "\n Avaliador removido com sucesso!\n";
    }
    else {
        cout << "\nOperação cancelada.\n";
    }
}


//  Autenticação (CPF e SENHA)

bool autenticarAvaliador(const string& cpf, const string& senha) {
    auto lista = carregarAvaliadores();
    string cpfDigitado = limparCPF(cpf);

    for (const auto& a : lista) {
        if (limparCPF(a.cpf) == cpfDigitado) {
            if (a.senha == senha)
                return true;
            else
                throw ExcecaoSenhaIncorreta("Senha incorreta!");
        }
    }
    throw ExcecaoAutenticacao("CPF não encontrado!");
}


//  Login pelo CPF

void loginAvaliador() {
    string cpf, senha;
    cout << "\n=== Login de Avaliador ===\n";
    cout << "CPF: ";
    getline(cin, cpf);
    cout << "Senha: ";
    getline(cin, senha);

    try {
        if (autenticarAvaliador(cpf, senha))
            cout << "\n? Login realizado com sucesso!\n";
    }
    catch (const exception& e) {
        cout << "[Erro de Login] " << e.what() << "\n";
    }
}
