#include "Entidades.h"
#include "Persistencia.h"
#include "Calculos.h"
#include "Paths.h"
#include <iostream>
#include <vector>

void registrarAvaliacao() {
    std::cout << "=== Registrar Avaliação ===\n";

    int projeto_id, avaliador_id;
    double n1, n2, n3, n4;
    std::string comentario;

    std::cout << "ID do projeto: ";
    std::cin >> projeto_id;
    std::cout << "ID do avaliador: ";
    std::cin >> avaliador_id;
    std::cout << "Notas (4 critérios separados por espaço): ";
    std::cin >> n1 >> n2 >> n3 >> n4;
    std::cin.ignore(); // limpa o buffer
    std::cout << "Comentário: ";
    std::getline(std::cin, comentario);

    std::vector<double> notas = {n1, n2, n3, n4};
    double media = mediaRecursiva(notas);

    Avaliacao av;
    av.id = rand() % 10000; // ID simples por enquanto
    av.projeto_id = projeto_id;
    av.avaliador_id = avaliador_id;
    av.n1 = n1; av.n2 = n2; av.n3 = n3; av.n4 = n4;
    av.comentario = comentario;
    av.media = media;

    // Lê avaliações já existentes

    auto avaliacoes = Persist::carregarAvaliacoes(dataDir() + "/avaliacoes.csv");
    avaliacoes.push_back(av);

    // Salva tudo

    Persist::salvarAvaliacoes(dataDir() + "/avaliacoes.csv", avaliacoes);

    std::cout << "\nAvaliação registrada com sucesso!\n";
    std::cout << "Média calculada (recursiva): " << media << "\n";
}
