// ===============================================
// SISTEMA DE PROJETOS
// ===============================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include "Projetos.hpp"
#include "Ficha.hpp"
#include "ID.hpp"

using namespace std;

const string ARQUIVO = "projetos.txt";

// ===============================================
// CARREGAR PROJETOS
// ===============================================
vector<Projeto> carregarProjetos() {
    vector<Projeto> lista;
    ifstream file(ARQUIVO);
    string linha;

    while (getline(file, linha)) {
        Projeto p;
        stringstream ss(linha);
        string campo;

        // Campos básicos
        getline(ss, campo, ';');
        p.id = stoi(campo);

        getline(ss, p.nome, ';');
        getline(ss, p.descricao, ';');
        getline(ss, p.responsavel, ';');
        getline(ss, p.categoria, ';');
        getline(ss, p.area, ';');
        getline(ss, p.tipoAvaliacao, ';');  // ✔ AGORA CARREGA CERTO

        // Avaliações
        for (int i = 0; i < 3; i++) {
            getline(ss, p.avaliacao[i].avaliadorCpf, ';');

            getline(ss, campo, ';');
            if (campo.empty()) campo = "0";
            p.avaliacao[i].nota = stod(campo);
        }

        // Nota final
        getline(ss, campo, ';');
        if (campo.empty()) campo = "-1";
        p.notaFinal = stod(campo);

        lista.push_back(p);
    }

    return lista;
}

// ===============================================
// SALVAR PROJETOS
// ===============================================
void salvarProjetos(const vector<Projeto>& lista) {
    ofstream file(ARQUIVO, ios::trunc);

    for (auto& p : lista) {
        file << p.id << ";"
            << p.nome << ";"
            << p.descricao << ";"
            << p.responsavel << ";"
            << p.categoria << ";"
            << p.area << ";"
            << p.tipoAvaliacao << ";";

        for (int i = 0; i < 3; i++) {
            file << p.avaliacao[i].avaliadorCpf << ";"
                << p.avaliacao[i].nota << ";";
        }

        file << p.notaFinal << "\n";
    }
}

// ===============================================
// CRIAR PROJETO
// ===============================================
void criarProjeto() {
    Projeto p;
    p.id = gerarNovoID();
    SistemaFichas s;
    carregarFichas(s);
    int index = -1;

    // Nome
    cout << "Nome (digite 0 para cancelar): ";
    getline(cin, p.nome);
    if (p.nome == "0") return;

    // Descricao
    cout << "Descricao (digite 0 para cancelar): ";
    getline(cin, p.descricao);
    if (p.descricao == "0") return;

    // Responsavel
    cout << "Responsavel (digite 0 para cancelar): ";
    getline(cin, p.responsavel);
    if (p.responsavel == "0") return;


    // Area
    cout << "Area: ";
    getline(cin, p.area);

    // Tipo de Avaliação
    do {
        cout << "Tipo de Avaliacao (digite 0 para cancelar): ";
        getline(cin, p.tipoAvaliacao);

        if (p.tipoAvaliacao == "0") return;

        Ficha* encontrada = buscarFichaPorTipo(s, p.tipoAvaliacao);
        if (encontrada != nullptr) {
            cout << "\nFicha encontrada: " << encontrada->tipoFicha << "\n";
            break;
        }

        cout << "Tipo invalido. Tentar novamente? (1 sim / 0 nao): ";
        string opc;
        getline(cin, opc);

        if (opc == "0") return;

    } while (true);

    cout << "tipo de avaliacao definido: " << p.tipoAvaliacao << "\n";

    // Salvar
    ofstream file(ARQUIVO, ios::app);
    file << p.id << ";" << p.nome << ";" << p.descricao << ";" << p.responsavel
        << ";" << p.categoria << ";" << p.area << ";" << p.tipoAvaliacao << ";"
        << ";;" << ";;" << ";;" // espaços para os 3 avaliadores
        << "-1" << "\n";
}

// ===============================================
// LISTAR PROJETOS
// ===============================================
void listarProjetos() {
    vector<Projeto> lista = carregarProjetos();

    cout << "\n📋 Lista de Projetos:\n";

    for (auto& p : lista) {
        cout << "----------------------------\n";
        cout << "ID: " << p.id << "\n";
        cout << "Nome: " << p.nome << "\n";
        cout << "Descricao: " << p.descricao << "\n";
        cout << "Responsavel: " << p.responsavel << "\n";
        cout << "Categoria: " << p.categoria << "\n";
        cout << "Area: " << p.area << "\n";
        cout << "Tipo de Avaliacao: " << p.tipoAvaliacao << "\n";

        for (int i = 0; i < 3; i++) {
            cout << "Avaliador " << i + 1 << ": ";
            if (p.avaliacao[i].avaliadorCpf.empty()) cout << "—";
            else cout << p.avaliacao[i].avaliadorCpf;

            cout << "  Nota: ";
            if (p.avaliacao[i].avaliadorCpf.empty()) cout << "—";
            else cout << p.avaliacao[i].nota;

            cout << "\n";
        }

        cout << "Nota Final: ";
        if (p.notaFinal == -1) cout << "—";
        else cout << p.notaFinal;

        cout << "\n";
    }
}

// ===============================================
// BUSCAR PROJETO PELO NOME
// ===============================================
Projeto buscarProjeto(const string& nome) {
    vector<Projeto> lista = carregarProjetos();

    for (auto& p : lista)
        if (p.nome == nome)
            return p;

    return Projeto{};
}

// ===============================================
// CONTAR AVALIAÇÕES JÁ FEITAS
// ===============================================
int contarAvaliacoes(const Projeto& p) {
    int qtd = 0;
    for (int i = 0; i < 3; i++)
        if (!p.avaliacao[i].avaliadorCpf.empty())
            qtd++;
    return qtd;
}

bool projetoFinalizado(const Projeto& p) {
    return contarAvaliacoes(p) >= 3;
}
string atualizarAvaliacaoRealizada(int idProjeto, const string& cpf, double nota) {
    vector<Projeto> lista = carregarProjetos();  // carrega tudo do arquivo

    for (auto& p : lista) {
        if (p.id == idProjeto) {

            int qtd = contarAvaliacoes(p);

            if (qtd == 3)
                return "Avaliacao do Projeto Finalizada, por favor escolha outro.";

            // Atualiza a avaliacao no projeto encontrado
            p.avaliacao[qtd].avaliadorCpf = cpf;
            p.avaliacao[qtd].nota = nota;

            // Se completar as 3 avaliações → calcular média final
            if (qtd + 1 == 3) {
                double soma = 0;
                for (int i = 0; i < 3; i++)
                    soma += p.avaliacao[i].nota;

                p.notaFinal = soma / 3.0;
            }

            // 🔥 Salva tudo novamente no arquivo
            salvarProjetos(lista);

            return "Avaliacao registrada e salva com sucesso!";
        }
    }

    return "Projeto nao encontrado.";
}

// ===============================================
// REGISTRAR AVALIAÇÃO
// ===============================================
string registrarAvaliacaoProjeto(int idProjeto, const string& cpf, double nota) {
    return atualizarAvaliacaoRealizada(idProjeto, cpf, nota);
}


// ===============================================
// PROJETO EXISTE?
// ===============================================
bool projetoExiste(const string& nome) {
    vector<Projeto> lista = carregarProjetos();

    for (auto& p : lista)
        if (p.nome == nome)
            return true;

    return false;
}

// ===============================================
// ATUALIZAR PROJETO
// ===============================================
void atualizarProjeto() {
    vector<Projeto> lista = carregarProjetos();
    int id;
    cout << "ID: ";
    cin >> id;
    cin.ignore();

    for (auto& p : lista) {
        if (p.id == id) {
            cout << "Novo nome: ";
            getline(cin, p.nome);

            cout << "Nova descricao: ";
            getline(cin, p.descricao);

            cout << "Novo responsavel: ";
            getline(cin, p.responsavel);

            salvarProjetos(lista);
            cout << "Atualizado!\n";
            return;
        }
    }

    cout << "Nao encontrado.\n";
}

// ===============================================
// DELETAR PROJETO
// ===============================================
void deletarProjeto() {
    vector<Projeto> lista = carregarProjetos();
    int id;
    cout << "ID: ";
    cin >> id;

    vector<Projeto> novaLista;

    for (auto& p : lista)
        if (p.id != id)
            novaLista.push_back(p);

    salvarProjetos(novaLista);
    cout << "Removido!\n";
}

