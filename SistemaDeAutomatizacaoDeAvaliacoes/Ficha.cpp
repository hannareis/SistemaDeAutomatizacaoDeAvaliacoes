#include "Ficha.hpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*
    Função responsável por inicializar o sistema de fichas.
    Define o número de fichas como zero e o ponteiro como nulo.
    Isso garante que o sistema comece “limpo”, sem dados pendentes.
*/
void inicializarSistema(SistemaFichas& s) {
    s.qtdFichas = 0;
    s.fichas = nullptr;
}

/*
    Verifica se já existe uma ficha técnica com o mesmo tipoFicha.
    Essa verificação impede que duas fichas iguais sejam criadas,
    pois tipoFicha atua como chave primária.
*/
bool tipoExiste(const SistemaFichas& s, const string& tipoFicha) {
    for (int i = 0; i < s.qtdFichas; i++) {
        if (s.fichas[i]->tipoFicha == tipoFicha) {
            return true;
        }
    }
    return false;
}

/*
    Cria uma nova ficha técnica e adiciona ao sistema.

    - Pergunta o tipo da ficha (ex: “TCC”, “Projeto”)
    - Impede duplicatas
    - Permite ao usuário cadastrar várias avaliações
    - Cada avaliação possui vários quesitos
*/
void criarFicha(SistemaFichas& s) {
    Ficha* nova = new Ficha; // Aloca dinamicamente uma nova ficha

    cout << "\n=== Criar nova Ficha Técnica ===\n";
    cout << "Digite o tipo da ficha (TCC, Projeto, etc): ";
    getline(cin, nova->tipoFicha);

    // Verifica se já existe uma ficha com o mesmo tipo
    if (tipoExiste(s, nova->tipoFicha)) {
        cout << "\n Já existe uma ficha com esse tipo! Operação cancelada.\n";
        delete nova;
        return;
    }

    cout << "Quantos tipos de avaliacao deseja adicionar? ";
    cin >> nova->qtdAvaliacoes;
    cin.ignore();

    // Aloca dinamicamente o vetor de avaliações
    nova->avaliacoes = new Avaliacao[nova->qtdAvaliacoes];

    for (int i = 0; i < nova->qtdAvaliacoes; i++) {
        Avaliacao& a = nova->avaliacoes[i];
        cout << "\n--- Avaliacao " << i + 1 << " ---\n";
        cout << "Tipo de avaliacao: ";
        getline(cin, a.tipo);

        cout << "Quantos quesitos tem essa avaliacao? ";
        cin >> a.qtdQuesitos;
        cin.ignore();

        // Aloca os quesitos dinamicamente
        a.quesitos = new Quesito[a.qtdQuesitos];

        for (int j = 0; j < a.qtdQuesitos; j++) {
            Quesito& q = a.quesitos[j];

            cout << "\nQuesito " << j + 1 << ": ";
            getline(cin, q.nome);

            cout << "Nota máxima possível para '" << q.nome << "': ";
            cin >> q.notaMax;
            cin.ignore();

            cout << "Esse quesito tem peso? (1 - Sim, 0 - Não): ";
            cin >> q.temPeso;

            if (q.temPeso) {
                cout << "Peso do quesito '" << q.nome << "': ";
                cin >> q.peso;
            }
            else {
                q.peso = 1.0; // Caso não tenha peso, valor padrão
            }
            cin.ignore();
        }
    }

    // Realoca o vetor de fichas para adicionar a nova
    Ficha** temp = new Ficha * [s.qtdFichas + 1];
    for (int i = 0; i < s.qtdFichas; i++)
        temp[i] = s.fichas[i];
    temp[s.qtdFichas] = nova;

    delete[] s.fichas; // libera o antigo vetor
    s.fichas = temp;
    s.qtdFichas++;

    cout << "\n✅ Ficha criada com sucesso!\n";
}

/*
    Lista todas as fichas e seus respectivos dados na tela.
    Percorre toda a estrutura (ficha → avaliação → quesito)
*/
void listarFichas(const SistemaFichas& s) {
    if (s.qtdFichas == 0) {
        cout << "\nNenhuma ficha cadastrada.\n";
        return;
    }

    cout << "\n=== Lista de Fichas Técnicas ===\n";
    for (int i = 0; i < s.qtdFichas; i++) {
        const Ficha* f = s.fichas[i];
        cout << "\nFicha: " << f->tipoFicha << "\n";

        for (int j = 0; j < f->qtdAvaliacoes; j++) {
            const Avaliacao& a = f->avaliacoes[j];
            cout << "  Avaliacao: " << a.tipo << "\n";

            for (int k = 0; k < a.qtdQuesitos; k++) {
                const Quesito& q = a.quesitos[k];
                cout << "    - " << q.nome
                    << " | Nota Max: " << q.notaMax
                    << " | Peso: " << (q.temPeso ? to_string(q.peso) : "N/A")
                    << "\n";
            }
        }
    }
    cout << "==============================\n";
}

/*
    Libera toda a memória alocada dinamicamente.
    (como não usamos classes, o "destrutor manual" é necessário)
*/
void liberarSistema(SistemaFichas& s) {
    for (int i = 0; i < s.qtdFichas; i++) {
        Ficha* f = s.fichas[i];
        for (int j = 0; j < f->qtdAvaliacoes; j++) {
            delete[] f->avaliacoes[j].quesitos;
        }
        delete[] f->avaliacoes;
        delete f;
    }

    delete[] s.fichas;
    s.fichas = nullptr;
    s.qtdFichas = 0;
}

/*
    Salva todos os dados do sistema em um arquivo texto (fichas.txt),
    para manter persistência entre execuções.
*/
void salvarFichas(const SistemaFichas& s) {
    ofstream arquivo(ARQUIVO_FICHAS);
    if (!arquivo.is_open()) {
        cout << " Erro ao abrir arquivo para salvar.\n";
        return;
    }

    arquivo << s.qtdFichas << "\n";
    for (int i = 0; i < s.qtdFichas; i++) {
        Ficha* f = s.fichas[i];
        arquivo << f->tipoFicha << "\n";
        arquivo << f->qtdAvaliacoes << "\n";

        for (int j = 0; j < f->qtdAvaliacoes; j++) {
            Avaliacao& a = f->avaliacoes[j];
            arquivo << a.tipo << "\n";
            arquivo << a.qtdQuesitos << "\n";

            for (int k = 0; k < a.qtdQuesitos; k++) {
                Quesito& q = a.quesitos[k];
                arquivo << q.nome << "\n";
                arquivo << q.notaMax << "\n";
                arquivo << q.temPeso << "\n";
                arquivo << q.peso << "\n";
            }
        }
    }

    arquivo.close();
    cout << "Fichas salvas em \"" << ARQUIVO_FICHAS << "\".\n";
}

/*
    Carrega os dados de um arquivo texto (fichas.txt)
    e reconstrói toda a estrutura em memória.
*/
void carregarFichas(SistemaFichas& s) {
    ifstream arquivo(ARQUIVO_FICHAS);
    if (!arquivo.is_open()) {
        cout << "(Nenhum arquivo encontrado, sistema iniciando vazio.)\n";
        return;
    }

    arquivo >> s.qtdFichas;
    arquivo.ignore();
    if (s.qtdFichas <= 0) {
        s.fichas = nullptr;
        return;
    }

    s.fichas = new Ficha * [s.qtdFichas];
    for (int i = 0; i < s.qtdFichas; i++) {
        Ficha* f = new Ficha;
        getline(arquivo, f->tipoFicha);
        arquivo >> f->qtdAvaliacoes;
        arquivo.ignore();

        f->avaliacoes = new Avaliacao[f->qtdAvaliacoes];
        for (int j = 0; j < f->qtdAvaliacoes; j++) {
            Avaliacao& a = f->avaliacoes[j];
            getline(arquivo, a.tipo);
            arquivo >> a.qtdQuesitos;
            arquivo.ignore();

            a.quesitos = new Quesito[a.qtdQuesitos];
            for (int k = 0; k < a.qtdQuesitos; k++) {
                getline(arquivo, a.quesitos[k].nome);
                arquivo >> a.quesitos[k].notaMax;
                arquivo >> a.quesitos[k].temPeso;
                arquivo >> a.quesitos[k].peso;
                arquivo.ignore();
            }
        }
        s.fichas[i] = f;
    }

    arquivo.close();
   // cout << "✅ Fichas carregadas do arquivo \"" << ARQUIVO_FICHAS << "\".\n";
}


double calcularNotaFinal ( const Ficha& ficha, const double notas[][5], int numAvaliadores ) {
    double notaFinal = 0.0;
    int totalQuesitos = 0;
    for ( int i = 0; i < ficha.qtdAvaliacoes; ++i ) {
        const Avaliacao& avaliacao = ficha.avaliacoes[i];
        for ( int j = 0; j < avaliacao.qtdQuesitos; ++j ) {
            const Quesito& quesito = avaliacao.quesitos[j];
            double somaNotasQuesito = 0.0;
            for ( int k = 0; k < numAvaliadores; ++k ) {
                somaNotasQuesito += notas[k][totalQuesitos];
            }
            double mediaQuesito = somaNotasQuesito / numAvaliadores;
            if ( quesito.temPeso ) {
                notaFinal += mediaQuesito * quesito.peso;
            } else {
                notaFinal += mediaQuesito;
            }
            ++totalQuesitos;
        }
    }
    return notaFinal;
}

Ficha* buscarFichaPorTipo(SistemaFichas& s, const std::string& tipoFicha) {
    for (int i = 0; i < s.qtdFichas; i++) {
        if (s.fichas[i]->tipoFicha == tipoFicha) {
            return s.fichas[i]; // retorna o ponteiro para a ficha encontrada
        }
    }
    return nullptr; // caso não encontre
}
