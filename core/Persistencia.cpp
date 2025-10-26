#include "Persistencia.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Função auxiliar interna: divide uma linha CSV em partes
static std::vector<std::string> dividirLinha(const std::string& linha, char sep = ';') {
    std::vector<std::string> partes;
    std::stringstream ss(linha);
    std::string item;
    while (std::getline(ss, item, sep)) {
        partes.push_back(item);
    }
    return partes;
}

namespace Persist {

    std::vector<Avaliador> carregarAvaliadores(const std::string& caminho) {
        std::vector<Avaliador> lista;
        std::ifstream arq(caminho);
        std::string linha;
        bool cabecalho = true;

        while (std::getline(arq, linha)) {
            if (cabecalho) { cabecalho = false; continue; } // ignora a primeira linha
            if (linha.empty()) continue;

            auto partes = dividirLinha(linha);
            if (partes.size() < 5) continue;

            Avaliador a;
            a.id = std::stoi(partes[0]);
            a.nome = partes[1];
            a.email = partes[2];
            a.formacao = partes[3];
            a.area = partes[4];
            lista.push_back(a);
        }
        return lista;
    }

    std::vector<Projeto> carregarProjetos(const std::string& caminho) {
        std::vector<Projeto> lista;
        std::ifstream arq(caminho);
        std::string linha;
        bool cabecalho = true;

        while (std::getline(arq, linha)) {
            if (cabecalho) { cabecalho = false; continue; }
            if (linha.empty()) continue;

            auto partes = dividirLinha(linha);
            if (partes.size() < 4) continue;

            Projeto p;
            p.id = std::stoi(partes[0]);
            p.titulo = partes[1];
            p.area = partes[2];
            p.tipo = partes[3];
            lista.push_back(p);
        }
        return lista;
    }

    std::vector<Avaliacao> carregarAvaliacoes(const std::string& caminho) {
        std::vector<Avaliacao> lista;
        std::ifstream arq(caminho);
        std::string linha;
        bool cabecalho = true;

        while (std::getline(arq, linha)) {
            if (cabecalho) { cabecalho = false; continue; }
            if (linha.empty()) continue;

            auto partes = dividirLinha(linha);
            if (partes.size() < 9) continue;

            Avaliacao av;
            av.id = std::stoi(partes[0]);
            av.projeto_id = std::stoi(partes[1]);
            av.avaliador_id = std::stoi(partes[2]);
            av.n1 = std::stod(partes[3]);
            av.n2 = std::stod(partes[4]);
            av.n3 = std::stod(partes[5]);
            av.n4 = std::stod(partes[6]);
            av.comentario = partes[7];
            av.media = std::stod(partes[8]);
            lista.push_back(av);
        }
        return lista;
    }

    void salvarAvaliadores(const std::string& caminho, const std::vector<Avaliador>& lista) {
        std::ofstream arq(caminho);
        arq << "id;nome;email;formacao;area\n";
        for (const auto& a : lista) {
            arq << a.id << ";" << a.nome << ";" << a.email << ";" << a.formacao << ";" << a.area << "\n";
        }
    }

    void salvarProjetos(const std::string& caminho, const std::vector<Projeto>& lista) {
        std::ofstream arq(caminho);
        arq << "id;titulo;area;tipo\n";
        for (const auto& p : lista) {
            arq << p.id << ";" << p.titulo << ";" << p.area << ";" << p.tipo << "\n";
        }
    }

    void salvarAvaliacoes(const std::string& caminho, const std::vector<Avaliacao>& lista) {
        std::ofstream arq(caminho);
        arq << "id;projeto_id;avaliador_id;n1;n2;n3;n4;comentario;media_calculada\n";
        for (const auto& av : lista) {
            arq << av.id << ";" << av.projeto_id << ";" << av.avaliador_id << ";"
                << av.n1 << ";" << av.n2 << ";" << av.n3 << ";" << av.n4 << ";"
                << av.comentario << ";" << av.media << "\n";
        }
    }
}
