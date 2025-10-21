#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../SistemaDeAutomatizacaoDeAvaliacoes/Organizadores.hpp"
using namespace std;

const string ARQUIVO_ORG = "organizadores.txt";

// Lê todos os organizadores do arquivo
vector<Organizador> carregarOrganizadores() {
    vector<Organizador> lista;
    ifstream file(ARQUIVO_ORG);

    if (!file.is_open()) return lista;

    string linha;
    while (getline(file, linha)) {
        if (linha.empty()) continue;

        Organizador o;
        stringstream ss(linha);
        string temp;

        getline(ss, temp, ';');
        o.id = stoi(temp);
        getline(ss, o.nome, ';');
        getline(ss, o.area, ';');

        lista.push_back(o);
    }

    file.close();
    return lista;
}

// Salva todos os organizadores no arquivo
void salvarOrganizadores(const vector<Organizador>& lista) {
    ofstream file(ARQUIVO_ORG, ios::trunc);
    for (const auto& o : lista) {
        file << o.id << ";" << o.nome << ";" << o.area << "\n";
    }
    file.close();
}

// Cria um novo organizador
void criarOrganizador() {
    Organizador o;
    vector<Organizador> lista = carregarOrganizadores();
    o.id = lista.empty() ? 1 : lista.back().id + 1;

    cout << "\n=== Criar Novo Organizador ===\n";
    cout << "ID gerado automaticamente: " << o.id << "\n";

    cout << "Nome: ";
    cin.ignore();
    getline(cin, o.nome);

    cout << "Área: ";
    getline(cin, o.area);

    ofstream file(ARQUIVO_ORG, ios::app);
    if (file.is_open()) {
        file << o.id << ";" << o.nome << ";" << o.area << "\n";
        file.close();
        cout << "\nOrganizador criado com sucesso!\n";
    } else {
        cout << "\nErro ao abrir o arquivo!\n";
    }
}

// Lista todos os organizadores
void listarOrganizadores() {
    vector<Organizador> lista = carregarOrganizadores();

    cout << "\n=== Lista de Organizadores ===\n";
    if (lista.empty()) {
        cout << "(nenhum organizador cadastrado)\n";
        return;
    }

    for (const auto& o : lista) {
        cout << "----------------------------\n";
        cout << "ID: " << o.id << "\n";
        cout << "Nome: " << o.nome << "\n";
        cout << "Área: " << o.area << "\n";
    }
    cout << "----------------------------\n";
}

// Atualiza um organizador pelo ID
void atualizarOrganizador() {
    vector<Organizador> lista = carregarOrganizadores();
    int id;

    cout << "\n=== Atualizar Organizador ===\n";
    cout << "Digite o ID do organizador que deseja atualizar: ";
    cin >> id;
    cin.ignore();

    bool encontrado = false;
    for (auto& o : lista) {
        if (o.id == id) {
            cout << "Novo nome: ";
            getline(cin, o.nome);
            cout << "Nova área: ";
            getline(cin, o.area);
            encontrado = true;
            break;
        }
    }

    if (encontrado) {
        salvarOrganizadores(lista);
        cout << "\nOrganizador atualizado com sucesso!\n";
    } else {
        cout << "\nOrganizador não encontrado.\n";
    }
}

// Deleta um organizador
void deletarOrganizador() {
    vector<Organizador> lista = carregarOrganizadores();
    int id;

    cout << "\n=== Deletar Organizador ===\n";
    cout << "Digite o ID do organizador que deseja deletar: ";
    cin >> id;
    cin.ignore();

    bool encontrado = false;
    vector<Organizador> novaLista;

    for (const auto& o : lista) {
        if (o.id != id)
            novaLista.push_back(o);
        else
            encontrado = true;
    }

    if (encontrado) {
        salvarOrganizadores(novaLista);
        cout << "\nOrganizador removido com sucesso!\n";
    } else {
        cout << "\nOrganizador não encontrado.\n";
    }
}

// ===================== BUSCAS =====================

// Buscar por ID
void buscarPorID() {
    vector<Organizador> lista = carregarOrganizadores();
    int id;
    cout << "\nDigite o ID para buscar: ";
    cin >> id;
    cin.ignore();

    for (const auto& o : lista) {
        if (o.id == id) {
            cout << "\nOrganizador encontrado:\n";
            cout << "ID: " << o.id << "\nNome: " << o.nome << "\nÁrea: " << o.area << "\n";
            return;
        }
    }
    cout << "\nNenhum organizador com esse ID.\n";
}

// Buscar por nome
void buscarPorNome() {
    vector<Organizador> lista = carregarOrganizadores();
    string nome;
    cout << "\nDigite o nome (ou parte dele): ";
    cin.ignore();
    getline(cin, nome);

    bool encontrado = false;
    for (const auto& o : lista) {
        if (o.nome.find(nome) != string::npos) {
            cout << "\nID: " << o.id << "\nNome: " << o.nome << "\nÁrea: " << o.area << "\n";
            encontrado = true;
        }
    }

    if (!encontrado)
        cout << "\nNenhum organizador encontrado com esse nome.\n";
}

// Buscar por área
void buscarPorArea() {
    vector<Organizador> lista = carregarOrganizadores();
    string area;
    cout << "\nDigite a área (ou parte dela): ";
    cin.ignore();
    getline(cin, area);

    bool encontrado = false;
    for (const auto& o : lista) {
        if (o.area.find(area) != string::npos) {
            cout << "\nID: " << o.id << "\nNome: " << o.nome << "\nÁrea: " << o.area << "\n";
            encontrado = true;
        }
    }

    if (!encontrado)
        cout << "\nNenhum organizador encontrado nessa área.\n";
}
