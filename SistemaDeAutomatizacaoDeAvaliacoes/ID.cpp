#include "../SistemaDeAutomatizacaoDeAvaliacoes/ID.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

const string ARQUIVO = "projetos.txt";
//GERSON FEZ A FUNÇAO DE GERAR ID AUTOMATICO
int gerarNovoID() {
    ifstream file(ARQUIVO);
    string linha;
    int maiorID = 0;

    while (getline(file, linha)) {
        stringstream ss(linha);
        string campo;
        getline(ss, campo, ';');
        int id = stoi(campo);
        if (id > maiorID) {
            maiorID = id;
        }
    }

    return maiorID + 1;
}