#pragma once
#include <string>

struct Avaliador {
    int id;
    std::string nome;
    std::string email;
    std::string formacao;
    std::string area;
};

struct Projeto {
    int id;
    std::string titulo;
    std::string area;
    std::string tipo; // "PROJETO" ou "ARTIGO"
};

struct Avaliacao {
    int id;
    int projeto_id;
    int avaliador_id;
    double n1, n2, n3, n4;
    std::string comentario;
    double media;
};
