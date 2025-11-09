#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../SistemaDeAutomatizacaoDeAvaliacoes/Notas.hpp"
#include "ID.hpp"
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
		getline(ss, n.projeto, ';');
		lista.push_back(n);
	}
	return lista;
}

void salvarNotas(const vector<Notas>& lista) {
	ofstream file(ARQUIVO, ios::trunc);
	for (auto& n : lista) {
		file << n.id << ";" << n.nota << ";" << n.nome_avaliador << ";" << ";" << n.projeto << "\n";
	}
}
double medianotas(double notas[5]) {
	double soma = 0.0;
	for (int i = 0; i < 5; i++) {
		soma += notas[i];
	}
	return soma / 5.0;
}

void criarNotas() {
	Notas n;
	int i;
	string criterios[5] = { "Inovacao", "Viabilidade", "Impacto Social", "Sustentabilidade", "Apresentacao" };
	n.id = gerarNovoID();
	cout << "Nome do avaliador: ";
	getline(cin, n.nome_avaliador);
	cout << "Projeto avaliado: ";
	getline(cin, n.projeto);
	for(i=0; i<5; i++){
		cout << "Nota do criterio " << criterios[i] << ": ";
		cin >> n.notasCriterio[i];
	}
	n.nota = medianotas(n.notasCriterio);
	ofstream file(ARQUIVO, ios::app);
	file << n.id << ";" << n.nota << ";" << n.nome_avaliador << ";" << ";" << n.projeto << "\n";
	file.close();
	cout << "✅ Nota registrada com sucesso!\n";
}

void listarNotas()
{
}
