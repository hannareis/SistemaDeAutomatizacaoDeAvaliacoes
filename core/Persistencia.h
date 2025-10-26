#pragma once
#include "Entidades.h"
#include <vector>
#include <string>

namespace Persist {

    // Funções de carregamento
    std::vector<Avaliador> carregarAvaliadores(const std::string& caminho);
    std::vector<Projeto> carregarProjetos(const std::string& caminho);
    std::vector<Avaliacao> carregarAvaliacoes(const std::string& caminho);

    // Funções de salvamento
    void salvarAvaliadores(const std::string& caminho, const std::vector<Avaliador>& lista);
    void salvarProjetos(const std::string& caminho, const std::vector<Projeto>& lista);
    void salvarAvaliacoes(const std::string& caminho, const std::vector<Avaliacao>& lista);
}
