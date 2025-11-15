#ifndef FICHA_HPP          // Evita que o arquivo seja incluído mais de uma vez no mesmo projeto
#define FICHA_HPP

#include <iostream>        // Necessário para entrada e saída de dados (cout, cin)
#include <string>          // Necessário para usar o tipo string
#include <fstream>         // Necessário para manipular arquivos (ifstream/ofstream)

/*
    ================================================
    ESTRUTURAS DE DADOS DO SISTEMA DE FICHAS TÉCNICAS
    ================================================

    O objetivo desse módulo é representar, armazenar e manipular
    fichas de avaliação técnica (como de TCCs, projetos, etc.).
    Cada ficha contém uma lista de avaliações, e cada avaliação
    contém uma lista de quesitos com notas.
*/

// --------------------
// Estrutura Quesito
// --------------------
// Representa um item individual que será avaliado dentro de uma avaliação.
// Exemplo: "Originalidade" com nota 8.5.
struct Quesito {
    std::string nome;   // Nome ou descrição do quesito avaliado
    double notaMax;       // Nota atribuída a esse quesito
	bool temPeso;   // Indica se o quesito possui peso na avaliação
	double peso;      // Peso do quesito na avaliação (se aplicável)
};

// --------------------
// Estrutura Avaliacao
// --------------------
// Representa um conjunto de quesitos agrupados por tipo de avaliação.
// Exemplo: Avaliação "Metodologia" com 3 quesitos diferentes.
struct Avaliacao {
    std::string tipo;   // Tipo de avaliação (ex: "Metodologia", "Apresentação")
    Quesito* quesitos;  // Vetor dinâmico de quesitos (alocado em tempo de execução)
    int qtdQuesitos;    // Quantidade de quesitos dessa avaliação
};

// --------------------
// Estrutura Ficha
// --------------------
// Representa uma ficha técnica completa, que pode ter várias avaliações.
// O campo tipoFicha serve como "chave primária", garantindo que não existam duplicatas.
struct Ficha {
    std::string tipoFicha;  // Identificador único (ex: "TCC", "Projeto Integrador")
    Avaliacao* avaliacoes;  // Vetor dinâmico de avaliações pertencentes a essa ficha
    int qtdAvaliacoes;      // Quantidade de avaliações nessa ficha
};

// --------------------
// Estrutura SistemaFichas
// --------------------
// Representa o conjunto total de fichas cadastradas no sistema.
// Serve como "base de dados" em memória, que será carregada e salva em arquivo.
struct SistemaFichas {
    Ficha** fichas;   // Vetor dinâmico de ponteiros para fichas (cada posição aponta para uma Ficha)
    int qtdFichas;    // Quantidade total de fichas armazenadas
};

// ------------------------------------------------------
// Constante do arquivo usado para salvar os dados no disco
// ------------------------------------------------------
// Este nome é usado pelas funções de persistência para salvar/carregar as fichas.
const std::string ARQUIVO_FICHAS = "fichas.csv";


// ------------------------------------------------------
// Declaração das funções principais do sistema
// ------------------------------------------------------

// Inicializa o sistema, configurando o vetor de fichas como vazio
void inicializarSistema(SistemaFichas& s);

// Verifica se já existe uma ficha com o mesmo tipoFicha (para evitar duplicatas)


// Cria uma nova ficha técnica, com avaliações e quesitos definidos pelo usuário
void criarFicha(SistemaFichas& s);

// Lista todas as fichas cadastradas no sistema com seus detalhes
void listarFichas(const SistemaFichas& s);

// Libera toda a memória alocada dinamicamente (evita vazamentos de memória)
void liberarSistema(SistemaFichas& s);

// ------------------------------------------------------
// Funções de persistência (salvar e carregar do arquivo)
// ------------------------------------------------------

// Salva todas as fichas do sistema no arquivo "fichas.txt"
void salvarFichas(const SistemaFichas& s);

// Carrega as fichas previamente salvas no arquivo para a memória
void carregarFichas(SistemaFichas& s);

double calcularNotaFinal(const Ficha& ficha, const double notas[][5], int numAvaliadores);

Ficha* buscarFichaPorTipo(SistemaFichas& s, const std::string& tipoFicha);


#endif // Final da proteção contra múltiplas inclusões
