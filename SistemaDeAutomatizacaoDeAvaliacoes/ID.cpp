#include "ID.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Tem que bater com o arquivo usado em Projetos.cpp
static const string ARQUIVO = "projetos.csv";

// Lê recursivamente o arquivo e devolve o maior ID encontrado na primeira coluna
static int encontrarMaiorID(ifstream& file, int maiorID = 0) {
    string linha;

    if (!getline(file, linha)) {
        // Fim do arquivo
        return maiorID;
    }

    if (!linha.empty()) {
        stringstream ss(linha);
        string campoID;
        getline(ss, campoID, ';'); // pega só o primeiro campo (ID)

        try {
            int id = stoi(campoID);
            if (id > maiorID) {
                maiorID = id;
            }
        }
        catch (...) {
            // Se a linha estiver zoada, simplesmente ignora e segue
        }
    }

    // Chamada recursiva para a próxima linha
    return encontrarMaiorID(file, maiorID);
}

// Função principal: devolve o próximo ID disponível
int gerarNovoID() {
    ifstream file(ARQUIVO);

    // Se não existe ainda nenhum arquivo de projetos,
    // é porque você ainda vai cadastrar o primeiro -> ID = 1
    if (!file.is_open()) {
        return 1;
    }

    int maiorID = encontrarMaiorID(file, 0);
    file.close();

    // Próximo ID é sempre maiorID + 1
    return maiorID + 1;
}
