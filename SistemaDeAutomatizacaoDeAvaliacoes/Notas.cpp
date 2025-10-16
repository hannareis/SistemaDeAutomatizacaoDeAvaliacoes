#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../SistemaDeAutomatizacaoDeAvaliacoes/Notas.hpp"
using namespace std;

const string ARQUIVO = "notas.txt"; // usa o caminho local do projeto	
vector<Notas> carregarNotas() {
	vector<Notas> lista;
   //
	ifstream file(ARQUIVO);
	string linha;
	while (getline(file, linha)) {
		Notas n;
		stringstream ss(linha);
		getline(ss, linha, ';'); n.id = stoi(linha);
		getline(ss, linha, ';'); n.nota = stod(linha);
		getline(ss, n.nome_avaliador, ';');
		getline(ss, n.equipe, ';');
		getline(ss, n.projeto, ';');
		lista.push_back(n);
	}
	return lista;
}

void salvarNotas(const vector<Notas>& lista) {
	ofstream file(ARQUIVO, ios::trunc);
	for (auto& n : lista) {
		file << n.id << ";" << n.nota << ";" << n.nome_avaliador << ";" << n.equipe << ";" << n.projeto << "\n";
	}
}


void criarNotas() {
	Notas n;
	cout << "ID da nota: ";
	cin >> n.id; cin.ignore();
	cout << "Nota (0.0 - 10.0): ";
	cin >> n.nota; cin.ignore();
	cout << "Nome do avaliador: ";
	getline(cin, n.nome_avaliador);
	cout << "Equipe avaliada: ";
	getline(cin, n.equipe);
	cout << "Projeto avaliado: ";
	getline(cin, n.projeto);
	ofstream file(ARQUIVO, ios::app);
	file << n.id << ";" << n.nota << ";" << n.nome_avaliador << ";" << n.equipe << ";" << n.projeto << "\n";
	file.close();
	cout << "✅ Nota registrada com sucesso!\n";
}