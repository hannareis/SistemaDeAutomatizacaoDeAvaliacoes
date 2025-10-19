#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../SistemaDeAutomatizacaoDeAvaliacoes/Avaliador.hpp"
using namespace std;

const string ARQUIVO = "avaliadores.txt";

// Thiago: Essa função lê o arquivo "avaliadores.txt" e carrega todos os avaliadores salvos para um vetor
vector<Avaliador> carregarAvaliadores() {
    vector<Avaliador> lista;
    ifstream file(ARQUIVO);

    if (!file.is_open()) {
        return lista; // Thiago: Se o arquivo não existir, retorna uma lista vazia
    }

    string linha;
    while (getline(file, linha)) {
        if (linha.empty() || linha.length() < 5) continue; // Thiago: Ignora linhas inválidas

        Avaliador a;
        stringstream ss(linha);
        string temp;

        try {
            getline(ss, temp, ';');
            a.id = stoi(temp);
            getline(ss, a.nome, ';');
            getline(ss, a.email, ';');
            getline(ss, a.categoria, ';');
            getline(ss, a.area_especialidade, ';');
            lista.push_back(a);
        }
        catch (...) {
            continue;
        }
    }
    file.close();
    return lista;
}

// Thiago: Essa função grava todos os avaliadores no arquivo "avaliadores.txt"
void salvarAvaliadores(const vector<Avaliador>& lista) {
    ofstream file(ARQUIVO, ios::trunc);
    for (const auto& a : lista) {
        file << a.id << ";" << a.nome << ";" << a.email << ";"
            << a.categoria << ";" << a.area_especialidade << "\n";
    }
    file.close();
}

// Thiago: Essa função cria um novo avaliador e salva no arquivo
void criarAvaliador() {
    Avaliador a;

    // Thiago: Gera um ID automaticamente
    vector<Avaliador> lista = carregarAvaliadores();
    a.id = lista.empty() ? 1 : lista.back().id + 1;

    cout << "\n=== Criar Novo Avaliador ===\n";
    cout << "ID gerado automaticamente: " << a.id << "\n\n";

    cout << "Nome: ";
    cin.ignore();
    getline(cin, a.nome);

    cout << "Email: ";
    getline(cin, a.email);

    int opcaoCategoria;
    cout << "\nCategoria:\n";
    cout << "1. Curso Tecnico\n";
    cout << "2. Graduacao\n";
    cout << "Escolha: ";
    cin >> opcaoCategoria;
    cin.ignore();

    // Thiago: Define a categoria e permite escolher o curso por número
    if (opcaoCategoria == 1) {
        a.categoria = "Tecnico";
        cout << "\nCursos Técnicos disponíveis:\n";
        cout << "1. Automacao Industrial\n";
        cout << "2. Eletronica\n";
        cout << "3. Eletrotecnica\n";
        cout << "4. Informatica\n";
        cout << "5. Mecanica\n";
        cout << "6. Mecatronica\n";
        cout << "7. Qualidade\n";
        cout << "8. Logistica\n";
        cout << "9. Seguranca do Trabalho\n";

        int opcaoCurso;
        cout << "Escolha o curso (1-9): ";
        cin >> opcaoCurso;
        cin.ignore();

        switch (opcaoCurso) {
        case 1: a.area_especialidade = "Automacao Industrial"; break;
        case 2: a.area_especialidade = "Eletronica"; break;
        case 3: a.area_especialidade = "Eletrotecnica"; break;
        case 4: a.area_especialidade = "Informatica"; break;
        case 5: a.area_especialidade = "Mecanica"; break;
        case 6: a.area_especialidade = "Mecatronica"; break;
        case 7: a.area_especialidade = "Qualidade"; break;
        case 8: a.area_especialidade = "Logistica"; break;
        case 9: a.area_especialidade = "Seguranca do Trabalho"; break;
        default: a.area_especialidade = "Nao definido"; break;
        }
    }
    else {
        a.categoria = "Graduacao";
        cout << "\nCursos de Graduação disponíveis:\n";
        cout << "1. Administracao\n";
        cout << "2. Ciencia da Computacao\n";
        cout << "3. Engenharia da Computacao\n";
        cout << "4. Engenharia de Producao\n";
        cout << "5. Engenharia de Software\n";
        cout << "6. Engenharia Eletrica\n";
        cout << "7. Engenharia Mecanica\n";

        int opcaoCurso;
        cout << "Escolha o curso (1-7): ";
        cin >> opcaoCurso;
        cin.ignore();

        switch (opcaoCurso) {
        case 1: a.area_especialidade = "Administracao"; break;
        case 2: a.area_especialidade = "Ciencia da Computacao"; break;
        case 3: a.area_especialidade = "Engenharia da Computacao"; break;
        case 4: a.area_especialidade = "Engenharia de Producao"; break;
        case 5: a.area_especialidade = "Engenharia de Software"; break;
        case 6: a.area_especialidade = "Engenharia Eletrica"; break;
        case 7: a.area_especialidade = "Engenharia Mecanica"; break;
        default: a.area_especialidade = "Nao definido"; break;
        }
    }

    // Thiago: Salva no arquivo
    ofstream file(ARQUIVO, ios::app);
    if (file.is_open()) {
        file << a.id << ";" << a.nome << ";" << a.email << ";"
            << a.categoria << ";" << a.area_especialidade << "\n";
        file.close();
        cout << "\nAvaliador criado com sucesso!\n";
    }
    else {
        cout << "\nErro ao abrir arquivo!\n";
    }
}

// Thiago: As demais funções permanecem iguais
void listarAvaliadores() {
    vector<Avaliador> lista = carregarAvaliadores();

    cout << "\n=== Lista de Avaliadores ===\n";
    if (lista.empty()) {
        cout << "(nenhum avaliador cadastrado)\n";
        return;
    }

    for (const auto& a : lista) {
        cout << "----------------------------\n";
        cout << "ID: " << a.id << "\n";
        cout << "Nome: " << a.nome << "\n";
        cout << "Email: " << a.email << "\n";
        cout << "Categoria: " << a.categoria << "\n";
        cout << "Especialidade: " << a.area_especialidade << "\n";
    }
    cout << "----------------------------\n";
}

void atualizarAvaliador() {
    vector<Avaliador> lista = carregarAvaliadores();
    int id;

    cout << "\n=== Atualizar Avaliador ===\n";
    cout << "Digite o ID do avaliador que deseja atualizar: ";
    cin >> id;
    cin.ignore();

    bool encontrado = false;
    for (auto& a : lista) {
        if (a.id == id) {
            cout << "Novo nome: ";
            getline(cin, a.nome);
            cout << "Novo email: ";
            getline(cin, a.email);

            int opcao;
            cout << "\nNova categoria:\n";
            cout << "1. Curso Tecnico\n";
            cout << "2. Graduacao\n";
            cout << "Escolha: ";
            cin >> opcao;
            cin.ignore();

            a.categoria = (opcao == 1 ? "Tecnico" : "Graduacao");

            cout << "Nova area de especialidade: ";
            getline(cin, a.area_especialidade);
            encontrado = true;
            break;
        }
    }

    if (encontrado) {
        salvarAvaliadores(lista);
        cout << "\nAvaliador atualizado!\n";
    }
    else {
        cout << "\nAvaliador nao encontrado.\n";
    }
}

void deletarAvaliador() {
    vector<Avaliador> lista = carregarAvaliadores();
    int id;

    cout << "\n=== Deletar Avaliador ===\n";
    cout << "Digite o ID do avaliador que deseja deletar: ";
    cin >> id;
    cin.ignore();

    bool encontrado = false;
    vector<Avaliador> novaLista;

    for (const auto& a : lista) {
        if (a.id != id)
            novaLista.push_back(a);
        else
            encontrado = true;
    }

    if (encontrado) {
        salvarAvaliadores(novaLista);
        cout << "\nAvaliador removido com sucesso!\n";
    }
    else {
        cout << "\nAvaliador nao encontrado.\n";
    }
}