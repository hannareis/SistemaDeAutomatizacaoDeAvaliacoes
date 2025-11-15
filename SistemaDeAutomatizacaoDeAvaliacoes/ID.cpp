#include "../SistemaDeAutomatizacaoDeAvaliacoes/ID.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

const string ARQUIVO = "projetos.csv";

//GERSON FEZ A FUNÇAO DE GERAR ID AUTOMATICO
int encontrarMaiorID(ifstream& file, int maiorID = 0) {
    string linha;
    if (!getline(file, linha)) {
        return maiorID;
    }

    stringstream ss(linha);
    string campo;
    getline(ss, campo, ';');
    int id = stoi(campo);

    if (id > maiorID) {
        maiorID = id;
    }

    return encontrarMaiorID(file, maiorID);
}

// Função principal
int gerarNovoID() {
    ifstream file(ARQUIVO);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    int maiorID = encontrarMaiorID(file);
    file.close();
    return maiorID + 1;
}