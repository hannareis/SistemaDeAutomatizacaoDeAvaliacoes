#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../SistemaDeAutomatizacaoDeAvaliacoes/Projetos.hpp"
#include "../SistemaDeAutomatizacaoDeAvaliacoes/ID.hpp"
using namespace std;

const string ARQUIVO = "projetos.txt"; // usa o caminho local do projeto

vector<Projeto> carregarProjetos() {
    vector<Projeto> lista;
   //
    ifstream file(ARQUIVO);
    string linha;

    while (getline(file, linha)) {
        Projeto p;
        stringstream ss(linha);
        getline(ss, linha, ';'); p.id = stoi(linha);
        getline(ss, p.nome, ';');
        getline(ss, p.descricao, ';');
        getline(ss, p.responsavel, ';');
        lista.push_back(p);
    }

    return lista;
}

void salvarProjetos(const vector<Projeto>& lista) {
    ofstream file(ARQUIVO, ios::trunc);
    for (auto& p : lista) {
        file << p.id << ";" << p.nome << ";" << p.descricao << ";" << p.responsavel << "\n";
    }
}



void criarProjeto() {
    Projeto p;
    //cout << "ID do projeto: ";
     p.id = gerarNovoID();
    cout << "Nome: ";
    getline(cin, p.nome);
    cout << "Descricao: ";
    getline(cin, p.descricao);
    cout << "Responsavel: ";
    getline(cin, p.responsavel);

    ofstream file(ARQUIVO, ios::app);
    file << p.id << ";" << p.nome << ";" << p.descricao << ";" << p.responsavel << "\n";
    file.close();

    cout << "✅ Projeto criado com sucesso!\n";
}

void listarProjetos() {
    vector<Projeto> lista = carregarProjetos();

    cout << "\n📋 Lista de Projetos:\n";
    for (auto& p : lista) {
        cout << "----------------------------\n";
        cout << "ID: " << p.id << "\n";
        cout << "Nome: " << p.nome << "\n";
        cout << "Descricao: " << p.descricao << "\n";
        cout << "Responsavel: " << p.responsavel << "\n";
    }
    if (lista.empty())
        cout << "(nenhum projeto cadastrado)\n";
}

void atualizarProjeto() {
    vector<Projeto> lista = carregarProjetos();
    int id;
    cout << "Digite o ID do projeto que deseja atualizar: ";
    cin >> id; cin.ignore();

    bool encontrado = false;
    for (auto& p : lista) {
        if (p.id == id) {
            cout << "Novo nome: ";
            getline(cin, p.nome);
            cout << "Nova descricao: ";
            getline(cin, p.descricao);
            cout << "Novo responsavel: ";
            getline(cin, p.responsavel);
            encontrado = true;
            break;
        }
    }

    if (encontrado) {
        salvarProjetos(lista);
        cout << "✅ Projeto atualizado!\n";
    }
    else {
        cout << "❌ Projeto não encontrado.\n";
    }
}

void deletarProjeto() {
    vector<Projeto> lista = carregarProjetos();
    int id;
    cout << "Digite o ID do projeto que deseja deletar: ";
    cin >> id;

    bool encontrado = false;
    vector<Projeto> novaLista;
    for (auto& p : lista) {
        if (p.id != id) {
            novaLista.push_back(p);
        }
        else {
            encontrado = true;
        }
    }

    if (encontrado) {
        salvarProjetos(novaLista);
        cout << "🗑️ Projeto removido com sucesso!\n";
    }
    else {
        cout << "❌ Projeto não encontrado.\n";
    }
}
