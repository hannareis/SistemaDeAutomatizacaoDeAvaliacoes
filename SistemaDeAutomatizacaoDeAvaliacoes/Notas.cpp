#include "Notas.hpp"
#include "Projetos.hpp"
#include "Ficha.hpp"
#include "Avaliador.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;

static const string ARQUIVO_NOTAS_CSV = "avaliacoes.csv";

// Verifica se o arquivo está vazio ou não existe
static bool arquivoVazio(const string& caminho) {
    ifstream f(caminho);
    if (!f.good()) return true;
    return f.peek() == ifstream::traits_type::eof();
}

// Cria uma linha CSV com tudo já vinculado (avaliador + projeto + nota final)
static void salvarNotaCSV(const Avaliador& av, const Projeto& proj, double notaFinal) {
    bool vazio = arquivoVazio(ARQUIVO_NOTAS_CSV);

    ofstream out(ARQUIVO_NOTAS_CSV, ios::app);
    if (!out.is_open()) {
        cout << "[Erro] Não foi possível abrir '" << ARQUIVO_NOTAS_CSV << "' para escrita.\n";
        return;
    }

    if (vazio) {
        out << "cpf_avaliador;"
            << "nome_avaliador;"
            << "categoria_avaliador;"
            << "area_avaliador;"
            << "id_projeto;"
            << "nome_projeto;"
            << "responsavel_projeto;"
            << "categoria_projeto;"
            << "area_projeto;"
            << "tipo_ficha;"
            << "nota_final\n";
    }

    out << av.cpf << ';'
        << av.nome << ';'
        << av.categoria << ';'
        << av.areaEspecialidade << ';'
        << proj.id << ';'
        << proj.nome << ';'
        << proj.responsavel << ';'
        << proj.categoria << ';'
        << proj.area << ';'
        << proj.tipoFicha << ';'
        << notaFinal << '\n';
}

// ==== Fluxo para o avaliador lançar nota em um projeto da própria área ====
void criarNotas() {
    const Avaliador* avLogado = obterAvaliadorLogado();
    if (!avLogado) {
        cout << "\n[Erro] Nenhum avaliador está logado. Faça login primeiro.\n";
        return;
    }
    const Avaliador& av = *avLogado;

    // 1. Carregar projetos compatíveis com a área/categoria do avaliador
    vector<Projeto> todos = carregarProjetos();
    vector<Projeto> filtrados;

    for (const auto& p : todos) {
        if (p.categoria == av.categoria && p.area == av.areaEspecialidade) {
            filtrados.push_back(p);
        }
    }

    if (filtrados.empty()) {
        cout << "\nNão há projetos cadastrados para sua área ("
            << av.categoria << " - " << av.areaEspecialidade << ").\n";
        return;
    }

    cout << "\n=== Projetos disponíveis para avaliação ===\n";
    for (size_t i = 0; i < filtrados.size(); ++i) {
        const auto& p = filtrados[i];
        cout << (i + 1) << ") [ID " << p.id << "] "
            << p.nome << " | " << p.categoria << " - " << p.area
            << " | Ficha: " << p.tipoFicha << "\n";
    }
    cout << "0) Cancelar\n";

    int escolha = -1;
    while (true) {
        cout << "Escolha o número do projeto: ";
        if (!(cin >> escolha)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Digite um número.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (escolha == 0) {
            cout << "Operação cancelada.\n";
            return;
        }
        if (escolha < 0 || escolha > static_cast<int>(filtrados.size())) {
            cout << "Opção inválida.\n";
            continue;
        }
        break;
    }

    Projeto proj = filtrados[escolha - 1];

    // 2. Carregar a ficha associada ao projeto
    SistemaFichas sistema;
    inicializarSistema(sistema);
    carregarFichas(sistema);

    Ficha* ficha = buscarFichaPorTipo(sistema, proj.tipoFicha);
    if (!ficha) {
        cout << "\n[Erro] Não existe ficha do tipo '" << proj.tipoFicha
            << "' cadastrada. Peça ao organizador para criar.\n";
        liberarSistema(sistema);
        return;
    }

    // 3. Coletar notas para cada quesito da ficha
    int totalQuesitos = 0;
    for (int i = 0; i < ficha->qtdAvaliacoes; ++i)
        totalQuesitos += ficha->avaliacoes[i].qtdQuesitos;

    vector<double> notasQuesitos(totalQuesitos, 0.0);

    cout << "\n=== Lançamento de notas para o projeto '" << proj.nome << "' ===\n";
    int idx = 0;
    for (int i = 0; i < ficha->qtdAvaliacoes; ++i) {
        Avaliacao& a = ficha->avaliacoes[i];
        cout << "\n--- Avaliação: " << a.tipo << " ---\n";
        for (int j = 0; j < a.qtdQuesitos; ++j) {
            Quesito& q = a.quesitos[j];
            double nota;
            while (true) {
                cout << "Nota para \"" << q.nome << "\" (0 a " << q.notaMax << "): ";
                if (!(cin >> nota)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Entrada inválida. Digite um número.\n";
                    continue;
                }
                if (nota < 0 || nota > q.notaMax) {
                    cout << "Nota fora do intervalo permitido.\n";
                    continue;
                }
                break;
            }
            notasQuesitos[idx++] = nota;
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // 4. Calcular nota final com base na ficha (usando peso quando tiver)
    double notaFinal = 0.0;
    idx = 0;
    for (int i = 0; i < ficha->qtdAvaliacoes; ++i) {
        Avaliacao& a = ficha->avaliacoes[i];
        for (int j = 0; j < a.qtdQuesitos; ++j) {
            Quesito& q = a.quesitos[j];
            double n = notasQuesitos[idx++];
            if (q.temPeso)
                notaFinal += n * q.peso;
            else
                notaFinal += n;
        }
    }

    cout << "\nNota final do projeto '" << proj.nome << "' = " << notaFinal << "\n";

    // 5. Registrar no CSV final
    salvarNotaCSV(av, proj, notaFinal);

    // Liberar memória das fichas
    liberarSistema(sistema);

    cout << "Avaliação registrada e salva em '" << ARQUIVO_NOTAS_CSV << "'.\n";
}

// ==== Listar notas a partir do CSV ====
void listarNotas() {
    ifstream in(ARQUIVO_NOTAS_CSV);
    if (!in.is_open()) {
        cout << "\nNenhum arquivo de avaliações encontrado ainda.\n";
        return;
    }

    string linha;
    bool primeira = true;
    cout << "\n=== Avaliações Registradas ===\n";
    while (getline(in, linha)) {
        if (linha.empty()) continue;
        if (primeira) { // pula o cabeçalho do CSV
            primeira = false;
            continue;
        }

        stringstream ss(linha);

        string cpf, nome, catAv, areaAv;
        string idProj, nomeProj, respProj, catProj, areaProj, tipoFicha, notaStr;

        getline(ss, cpf, ';');
        getline(ss, nome, ';');
        getline(ss, catAv, ';');
        getline(ss, areaAv, ';');
        getline(ss, idProj, ';');
        getline(ss, nomeProj, ';');
        getline(ss, respProj, ';');
        getline(ss, catProj, ';');
        getline(ss, areaProj, ';');
        getline(ss, tipoFicha, ';');
        getline(ss, notaStr, ';'); // última coluna

        cout << "\nAvaliador: " << nome << " (CPF " << cpf << ") - " << catAv << " - " << areaAv
            << "\nProjeto: [" << idProj << "] " << nomeProj
            << "\nÁrea projeto: " << catProj << " - " << areaProj
            << "\nFicha: " << tipoFicha
            << "\nNota final: " << notaStr
            << "\n------------------------------------";
    }
    cout << "\n";
}
