#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>   // para pegar a data atual
#include "../SistemaDeAutomatizacaoDeAvaliacoes/Notas.hpp"
#include "ID.hpp"
#include "Ficha.hpp"
using namespace std;

const string ARQUIVO = "notas.txt"; // usa o caminho local do projeto	


// --- Carrega notas do arquivo ---
vector<Notas> carregarNotas() {
    vector<Notas> lista;
    ifstream file(ARQUIVO);
    string linha;

    while (getline(file, linha)) {
        Notas n;
        stringstream ss(linha);
        string campo;

        getline(ss, n.data, ';');
        getline(ss, campo, ';'); n.nota = stod(campo);
        getline(ss, n.tipo_avaliacao, ';');
        getline(ss, n.nome_avaliador, ';');
        getline(ss, n.projeto, ';');

        lista.push_back(n);
    }
    return lista;
}

// --- Salva todas as notas no arquivo ---
void salvarNotas(const vector<Notas>& lista) {
    ofstream file(ARQUIVO, ios::trunc);
    for (const auto& n : lista) {
        file << n.data << ";"
            << n.nota << ";"
            << n.tipo_avaliacao << ";"
            << n.nome_avaliador << ";"
            << n.projeto << "\n";
    }
}

// --- Calcula a média de 5 critérios ---
double mediaNotas(double notas[], int indiceNota) {
    double soma = 0.0;
    for (int i = 0; i < indiceNota; i++) {
        soma += notas[i];
    }

    double media = (indiceNota > 0) ? soma / indiceNota : 0;

    cout << "\n Todas as notas foram registradas!\n";
    cout << "Média final: " << media << "\n";
    return media;

}
void registrarNotas(SistemaFichas& s, const string& tipoAvaliacao) {
    Ficha* encontrada = buscarFichaPorTipo(s, tipoAvaliacao);

    if (encontrada != nullptr) {
        cout << "\n📋 Ficha encontrada: " << encontrada->tipoFicha << "\n";

        // número total de quesitos (vamos descobrir percorrendo tudo)
        int totalQuesitos = 0;
        for (int j = 0; j < encontrada->qtdAvaliacoes; j++) {
            totalQuesitos += encontrada->avaliacoes[j].qtdQuesitos;
        }

        // vetor auxiliar de notas (tamanho total dos quesitos)
        double* notas = new double[totalQuesitos];
        int indiceNota = 0;

        // percorrer as avaliações e coletar notas
        for (int j = 0; j < encontrada->qtdAvaliacoes; j++) {
            const Avaliacao& a = encontrada->avaliacoes[j];
            
            // se quiser filtrar

            cout << "  Avaliação: " << a.tipo << "\n";

            for (int k = 0; k < a.qtdQuesitos; k++) {
                const Quesito& q = a.quesitos[k];
                cout << "    - " << q.nome << " | Nota Máx: " << q.notaMax << "\n";
                cout << "      Digite a nota: ";

                cin >> notas[indiceNota];

                while (cin.fail() || notas[indiceNota] < 0 || notas[indiceNota] > q.notaMax) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "      ⚠️ Valor inválido! Digite novamente: ";
                    cin >> notas[indiceNota];
                }

                indiceNota++;
            }
        }


        // calcula média
        mediaNotas(notas, indiceNota);
//levar em consideracao que pode ser uma media ponderada
  //mostrar os pesos se tiver
        // associar a um projeto
        // guardar o cpf do avaliador quando ele entrar
        //organizar o fluxo inteiro
        //organizar as pastas

    }
    else {
        cout << "\n Nenhuma ficha encontrada com esse tipo.\n";
    }
}

// --- Cria e grava nova avaliação ---
void criarNotas() {
    Notas n;
  

    n.data = "24/12/2025";  // pega a data atual
    n.nota = 0.0;

    cout << "Nome do avaliador: ";
    getline(cin, n.nome_avaliador);

    cout << "Projeto avaliado: ";
    getline(cin, n.projeto);

    cout << "Tipo de avaliacao (ex: Parcial, Final, etc.): ";
    getline(cin, n.tipo_avaliacao);

    SistemaFichas s;
    carregarFichas(s);

    registrarNotas(s, n.tipo_avaliacao);


    ofstream file(ARQUIVO, ios::app);
    file << n.data << ";"
        << n.nota << ";"
        << n.tipo_avaliacao << ";"
        << n.nome_avaliador << ";"
        << n.projeto << "\n";
    file.close();

    cout << "Nota registrada com sucesso em " << n.data << "!\n";
}



// --- Listar notas registradas ---
void listarNotas() {
    vector<Notas> lista = carregarNotas();
    cout << "\n=== LISTA DE NOTAS ===\n";
    for (const auto& n : lista) {
        cout << "Data: " << n.data
            << " | Tipo: " << n.tipo_avaliacao
            << " | Avaliador: " << n.nome_avaliador
            << " | Projeto: " << n.projeto
            << " | Nota final: " << n.nota << endl;
    }
}
