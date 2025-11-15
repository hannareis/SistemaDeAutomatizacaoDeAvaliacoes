#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>   // para pegar a data atual
#include "Notas.hpp"
#include "ID.hpp"
#include "Ficha.hpp"
#include "Projetos.hpp"
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
double mediaNotas(double notas[], double pesos[], int quantidade) {
    double somaPonderada = 0.0;
    double somaPesos = 0.0;

    for (int i = 0; i < quantidade; i++) {
        somaPonderada += notas[i] * pesos[i];
        somaPesos += pesos[i];
    }

    double media = (somaPesos > 0) ? somaPonderada / somaPesos : 0.0;

    cout << "\nTodas as notas foram registradas!\n";
    cout << "Média final (ponderada): " << media << "\n";

    return media;
}


double registrarNotas(SistemaFichas& s, const string& tipoAvaliacao) {
    cout << "\n=== Registro de Notas para Ficha: " << tipoAvaliacao << " ===\n";

    Ficha* encontrada = buscarFichaPorTipo(s, tipoAvaliacao);

    if (encontrada != nullptr) {
        cout << "\n Ficha encontrada: " << encontrada->tipoFicha << "\n";

        // número total de quesitos (vamos descobrir percorrendo tudo)
        int totalQuesitos = 0;
        for (int j = 0; j < encontrada->qtdAvaliacoes; j++) {
            totalQuesitos += encontrada->avaliacoes[j].qtdQuesitos;
        }

        // vetor auxiliar de notas (tamanho total dos quesitos)
        double* notas = new double[totalQuesitos];
        double* pesos = new double[totalQuesitos];
        int indiceNota = 0;

        // percorrer as avaliações e coletar notas
        for (int j = 0; j < encontrada->qtdAvaliacoes; j++) {
            const Avaliacao& a = encontrada->avaliacoes[j];

            // se quiser filtrar

            cout << "  Avaliação: " << a.tipo << "\n";

            for (int k = 0; k < a.qtdQuesitos; k++) {
                pesos[indiceNota] = a.quesitos->peso;

                const Quesito& q = a.quesitos[k];
                cout << "    - " << q.nome << " | Nota Máx: " << q.notaMax << "| Peso: " << q.peso << "\n";
                cout << "      Digite a nota: ";

                cin >> notas[indiceNota];

                while (cin.fail() || notas[indiceNota] < 0 || notas[indiceNota] > q.notaMax) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << " Valor inválido! Digite novamente: ";
                    cin >> notas[indiceNota];
                }

                indiceNota++;
            }
        }



        return mediaNotas(notas, pesos, indiceNota);
        //mostrar os pesos se tiver
              // associar a um projeto
              // guardar o cpf do avaliador quando ele entrar
              //organizar o fluxo inteiro
              //organizar as pastas

    }
    else {
        cout << "\n Nenhuma ficha encontrada com esse tipo.\n";
        return 0.0;
    }
}

// --- Cria e grava nova avaliação ---
void criarNotas() {
    Notas n;


    n.data = "24/12/2025";  // pega a data atual
    n.nota = 0.0;

    cout << "Nome do avaliador: ";
    getline(cin, n.nome_avaliador);





    int index = -1;
    do {
        cout << "Projeto avaliado (0 para cancelar): ";
        getline(cin, n.projeto);

        if (n.projeto == "0") {
            cout << "Operacao cancelada pelo usuario.\n";
            return;
        }

        if (!projetoExiste(n.projeto)) {
            cout << "Projeto nao encontrado.\n";

            while (true) {
                cout << "Deseja tentar novamente? (1 - Sim / 0 - Nao): ";
                string escolha;
                getline(cin, escolha);

                if (escolha == "0") {
                    cout << "Operacao cancelada pelo usuario.\n";
                    return;
                }
                else if (escolha == "1") {
                    break; // tenta novamente
                }
                else {
                    cout << "Entrada invalida. Digite apenas 1 ou 0.\n";
                }
            }

            continue;
        }

        // -------------------- PROJETO ENCONTRADO --------------------
        Projeto p = buscarProjeto(n.projeto);

        // 🔥 VERIFICA ANTES DE QUALQUER PROCESSO DE FICHA
        if (projetoFinalizado(p)) {
            cout << "❌ Avaliacao do Projeto Finalizada (já existem 3 avaliações).\n";
            continue; // permite tentar outro projeto
        }

        // Mostra a ficha que será usada
        cout << "Ficha de avaliacao: " << p.tipoAvaliacao << "\n";

        // Carrega o conjunto de fichas do sistema
        SistemaFichas s;
        carregarFichas(s);
        cout << "Carregadas " << s.qtdFichas << " fichas do sistema.\n";

        n.tipo_avaliacao = p.tipoAvaliacao;

        cout << "tipo de avaliacao: " << n.tipo_avaliacao << "\n";
        // 📌 Somente aqui chamamos a ficha — DEPOIS da verificação!
        n.nota = registrarNotas(s, p.tipoAvaliacao);

        int qtd = contarAvaliacoes(p);
        string mensagem = registrarAvaliacaoProjeto(p.id, n.nome_avaliador, n.nota);
        cout << mensagem << "\n";

        // Se após registrar a nota chegou a 3 → não salva nota no TXT
        if (mensagem.find("Finalizada") != string::npos) {
            continue;
        }

        // Grava a nota no arquivo
        ofstream file(ARQUIVO, ios::app);
        file << n.data << ";"
            << n.nota << ";"
            << n.tipo_avaliacao << ";"
            << n.nome_avaliador << ";"
            << n.projeto << "\n";
        file.close();

        cout << "Nota registrada com sucesso em " << n.data << "!\n";
        break;

    } while (true);


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
