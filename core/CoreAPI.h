#pragma once
#include <vector>
#include <map>
#include <utility>
#include <string>
#include "Entidades.h"

struct ResumoProjeto {
    double media;
    int quantidade;
    std::vector<std::string> comentarios;
};

class CoreAPI {
public:
    // Carrega tudo (se precisar no futuro)
    void carregar();

    // Cadastros com ID automático
    int cadastrarAvaliador(const std::string& nome, const std::string& email,
                           const std::string& formacao, const std::string& area);
    int cadastrarProjeto(const std::string& titulo, const std::string& area,
                         const std::string& tipo); // "PROJETO" | "ARTIGO"

    // Registrar avaliação (calcula média recursiva automaticamente)
    int registrarAvaliacao(int projeto_id, int avaliador_id,
                           double n1, double n2, double n3, double n4,
                           const std::string& comentario);

    // Retorna ranking como vetor de (media, projeto_id) ordenado desc.
    std::vector<std::pair<double,int>> ranking();

    // Exporta Ranking para CSV
    void exportarRankingCSV();

    // Exibe os Detalhes dos Projetos Avaliados
    ResumoProjeto resumoProjeto(int projeto_id);

    //Reseta as Informações do Programa
    void resetSistema();

private:
    // Helpers internos
    int proximoIDAvaliador();
    int proximoIDProjeto();
    int proximoIDAvaliacao();

    // Carregadores
    std::vector<Avaliador> loadAvaliadores();
    std::vector<Projeto>   loadProjetos();
    std::vector<Avaliacao> loadAvaliacoes();

    // Salvadores
    void saveAvaliadores(const std::vector<Avaliador>& v);
    void saveProjetos(const std::vector<Projeto>& v);
    void saveAvaliacoes(const std::vector<Avaliacao>& v);
};
