#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>

#include "Projetos.hpp"
#include "ID.hpp"
#include "Menus.hpp"

using namespace std;

static const string ARQUIVO_PROJETOS = "projetos.txt"; // armazenamento interno (texto com ;)

vector<Projeto> carregarProjetos() {
    vector<Projeto> lista;
    ifstream file(ARQUIVO_PROJETOS);
    if (!file.is_open()) {
        // sem arquivo ainda -> lista vazia
        return lista;
    }

    string linha;
    while (getline(file, linha)) {
        if (linha.empty()) continue;

        Projeto p;
        string campo;
        stringstream ss(linha);

        // ID
        if (!getline(ss, campo, ';')) continue;
        try {
            p.id = stoi(campo);
        }
        catch (...) {
            continue;
        }

        getline(ss, p.nome, ';');
        getline(ss, p.descricao, ';');
        getline(ss, p.responsavel, ';');
        getline(ss, p.categoria, ';');
        getline(ss, p.area, ';');
        getline(ss, p.tipoFicha, ';');

        // se não tiver categoria/área/tipoFicha em linhas antigas, ficam vazios
        lista.push_back(p);
    }

    return lista;
}

void salvarProjetos(const vector<Projeto>& lista) {
    ofstream file(ARQUIVO_PROJETOS, ios::trunc);
    if (!file.is_open()) {
        cout << "[Erro] Não foi possível abrir '" << ARQUIVO_PROJETOS << "' para escrita.\n";
        return;
    }

    for (const auto& p : lista) {
        file << p.id << ';'
            << p.nome << ';'
            << p.descricao << ';'
            << p.responsavel << ';'
            << p.categoria << ';'
            << p.area << ';'
            << p.tipoFicha << '\n';
    }
}

// ==== CRUD ====

void criarProjeto() {
    Projeto p;
    p.id = gerarNovoID();

    cout << "\n=== Cadastro de Projeto ===\n";
    cout << "Nome: ";
    getline(cin, p.nome);
    cout << "Descrição: ";
    getline(cin, p.descricao);
    cout << "Responsável: ";
    getline(cin, p.responsavel);

    cout << "\nDefina a categoria e área do projeto:\n";
    if (!menuCategoriaEArea(p.categoria, p.area)) {
        cout << "Categoria/Área não selecionadas. Cancelando cadastro.\n";
        return;
    }

    cout << "Tipo de ficha associada (ex: TCC, Projeto Integrador): ";
    getline(cin, p.tipoFicha);

    ofstream file(ARQUIVO_PROJETOS, ios::app);
    if (!file.is_open()) {
        cout << "[Erro] Não foi possível abrir '" << ARQUIVO_PROJETOS << "' para escrita (append).\n";
        return;
    }

    file << p.id << ';'
        << p.nome << ';'
        << p.descricao << ';'
        << p.responsavel << ';'
        << p.categoria << ';'
        << p.area << ';'
        << p.tipoFicha << '\n';

    cout << "[OK] Projeto criado com sucesso!\n";
}

void listarProjetos() {
    vector<Projeto> lista = carregarProjetos();

    cout << "\nLista de Projetos:\n";
    if (lista.empty()) {
        cout << "(nenhum projeto cadastrado)\n";
        return;
    }

    for (const auto& p : lista) {
        cout << "----------------------------\n";
        cout << "ID: " << p.id << "\n";
        cout << "Nome: " << p.nome << "\n";
        cout << "Descrição: " << p.descricao << "\n";
        cout << "Responsável: " << p.responsavel << "\n";
        cout << "Categoria: " << p.categoria << "\n";
        cout << "Área: " << p.area << "\n";
        cout << "Ficha associada: " << p.tipoFicha << "\n";
    }
}

void atualizarProjeto() {
    vector<Projeto> lista = carregarProjetos();
    if (lista.empty()) {
        cout << "\nNenhum projeto cadastrado.\n";
        return;
    }

    int id;
    cout << "Digite o ID do projeto que deseja atualizar: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada inválida.\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool encontrado = false;
    for (auto& p : lista) {
        if (p.id == id) {
            encontrado = true;
            string tmp;

            cout << "Novo nome (vazio = manter): ";
            getline(cin, tmp);
            if (!tmp.empty()) p.nome = tmp;

            cout << "Nova descrição (vazio = manter): ";
            getline(cin, tmp);
            if (!tmp.empty()) p.descricao = tmp;

            cout << "Novo responsável (vazio = manter): ";
            getline(cin, tmp);
            if (!tmp.empty()) p.responsavel = tmp;

            cout << "\nDeseja alterar categoria/área? (1 = sim, 0 = não): ";
            int opCat = 0;
            if (cin >> opCat) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (opCat == 1) {
                    if (!menuCategoriaEArea(p.categoria, p.area)) {
                        cout << "Categoria/Área mantidas.\n";
                    }
                }
            }
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "Novo tipo de ficha (vazio = manter '" << p.tipoFicha << "'): ";
            getline(cin, tmp);
            if (!tmp.empty()) p.tipoFicha = tmp;

            break;
        }
    }

    if (!encontrado) {
        cout << "[ERRO] Projeto não encontrado.\n";
        return;
    }

    salvarProjetos(lista);
    cout << "[OK] Projeto atualizado!\n";
}

void deletarProjeto() {
    vector<Projeto> lista = carregarProjetos();
    if (lista.empty()) {
        cout << "\nNenhum projeto cadastrado.\n";
        return;
    }

    int id;
    cout << "Digite o ID do projeto que deseja deletar: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada inválida.\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool encontrado = false;
    vector<Projeto> novaLista;

    for (const auto& p : lista) {
        if (p.id == id) {
            encontrado = true;
            continue;
        }
        novaLista.push_back(p);
    }

    if (encontrado) {
        salvarProjetos(novaLista);
        cout << "[OK] Projeto removido com sucesso!\n";
    }
    else {
        cout << "[ERRO] Projeto não encontrado.\n";
    }
}
