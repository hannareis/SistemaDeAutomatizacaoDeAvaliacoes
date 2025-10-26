#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include "core/CoreAPI.h"
#include "core/Persistencia.h"  // para listar dados direto do CSV
#include "core/Paths.h"         // para dataDir()

// ------------ Utils de entrada ------------
static void limparEntrada() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ------------ Senha do organizador ------------
static const char* ADMIN_PASS = "admin123";

static bool pedirSenha() {
    std::string s;
    std::cout << "Senha do organizador: ";
    std::getline(std::cin, s);
    if (s == ADMIN_PASS) return true;
    std::cout << "Acesso negado.\n";
    return false;
}

// ------------ Seed de dados ------------
static void seed(CoreAPI& api) {
    int a1 = api.cadastrarAvaliador("Hanna Reis", "hanna@fucapi.com", "Graduacao", "Engenharia de Software");
    int a2 = api.cadastrarAvaliador("Thiago Lopes", "thiago@fucapi.com", "Graduacao", "Ciencia da Computacao");

    int p1 = api.cadastrarProjeto("Detector de Fake News", "Computacao", "PROJETO");
    int p2 = api.cadastrarProjeto("Artigo: Otimizacao com IA", "Computacao", "ARTIGO");

    api.registrarAvaliacao(p1, a1, 9, 8, 9, 10, "Muito bom");
    api.registrarAvaliacao(p1, a2, 8, 9, 8, 9,  "Apresentacao clara");
    api.registrarAvaliacao(p2, a1, 7, 8, 7, 8,  "Bons resultados");
    api.registrarAvaliacao(p2, a2, 6, 7, 8, 7,  "Metodologia ok");

    std::cout << "Seed aplicado: 2 avaliadores, 2 projetos e 4 avaliacoes.\n";
}

// ------------ Listagens (direto dos CSVs) ------------
static void listarAvaliadores() {
    auto lista = Persist::carregarAvaliadores(dataDir() + "/avaliadores.csv");
    if (lista.empty()) {
        std::cout << "(nenhum avaliador cadastrado)\n";
        return;
    }
    std::cout << "=== Avaliadores Cadastrados ===\n";
    for (const auto& a : lista) {
        std::cout << "ID " << a.id << " | " << a.nome
                  << " | " << a.email
                  << " | " << a.formacao
                  << " | " << a.area << "\n";
    }
}

static void listarProjetos() {
    auto lista = Persist::carregarProjetos(dataDir() + "/projetos.csv");
    if (lista.empty()) {
        std::cout << "(nenhum projeto cadastrado)\n";
        return;
    }
    std::cout << "=== Projetos Cadastrados ===\n";
    for (const auto& p : lista) {
        std::cout << "ID " << p.id << " | " << p.titulo
                  << " | " << p.area
                  << " | " << p.tipo << "\n";
    }
}

int main() {
    CoreAPI api;
    api.carregar();

    while (true) {
        std::cout << "\n=== AvaliaEncoeng (CLI) ===\n";
        std::cout << "1) Cadastrar avaliador / Listar avaliadores\n";
        std::cout << "2) Cadastrar projeto / Listar projetos\n";
        std::cout << "3) Registrar avaliacao\n";
        std::cout << "4) Ver ranking\n";
        std::cout << "5) Popular dados de exemplo (seed)\n";
        std::cout << "6) Exportar ranking para CSV\n";
        std::cout << "7) Detalhes de um projeto (média + comentários)\n";
        std::cout << "8) Resetar Sistema (limpar CSVs)\n";
        std::cout << "0) Sair\n";
        std::cout << "Escolha: ";

        int op;
        if (!(std::cin >> op)) { limparEntrada(); continue; }
        limparEntrada();

        if (op == 0) {
            std::cout << "Saindo...\n";
            break;
        }
        else if (op == 1) {
            if (!pedirSenha()) continue;

            while (true) {
                std::cout << "\n--- Avaliadores ---\n";
                std::cout << "1) Cadastrar novo avaliador\n";
                std::cout << "2) Listar avaliadores\n";
                std::cout << "0) Voltar\n";
                std::cout << "Escolha: ";
                int s; 
                if (!(std::cin >> s)) { limparEntrada(); continue; }
                limparEntrada();

                if (s == 0) break;
                else if (s == 1) {
                    std::string nome, email, formacao, area;
                    std::cout << "Nome: ";        std::getline(std::cin, nome);
                    std::cout << "Email: ";       std::getline(std::cin, email);
                    std::cout << "Formacao: ";    std::getline(std::cin, formacao);
                    std::cout << "Area: ";        std::getline(std::cin, area);
                    int id = api.cadastrarAvaliador(nome, email, formacao, area);
                    std::cout << "Avaliador cadastrado com ID " << id << "\n";
                } else if (s == 2) {
                    listarAvaliadores();
                } else {
                    std::cout << "Opcao invalida.\n";
                }
            }
        }
        else if (op == 2) {
            if (!pedirSenha()) continue;

            while (true) {
                std::cout << "\n--- Projetos ---\n";
                std::cout << "1) Cadastrar novo projeto\n";
                std::cout << "2) Listar projetos\n";
                std::cout << "0) Voltar\n";
                std::cout << "Escolha: ";
                int s; 
                if (!(std::cin >> s)) { limparEntrada(); continue; }
                limparEntrada();

                if (s == 0) break;
                else if (s == 1) {
                    std::string titulo, area, tipo;
                    std::cout << "Titulo: "; std::getline(std::cin, titulo);
                    std::cout << "Area: ";   std::getline(std::cin, area);
                    std::cout << "Tipo (PROJETO|ARTIGO): "; std::getline(std::cin, tipo);
                    int id = api.cadastrarProjeto(titulo, area, tipo);
                    std::cout << "Projeto cadastrado com ID " << id << "\n";
                } else if (s == 2) {
                    listarProjetos();
                } else {
                    std::cout << "Opcao invalida.\n";
                }
            }
        }
        else if (op == 3) {
            auto projetos = Persist::carregarProjetos(dataDir() + "/projetos.csv");
            if (projetos.empty()) {
                std::cout << "Nao ha projetos cadastrados. Cadastre em (2).\n";
                continue;
            }
            std::cout << "\n=== Escolha o Projeto ===\n";
            for (const auto& p : projetos) {
                std::cout << p.id << ") " << p.titulo << " | " << p.area << " | " << p.tipo << "\n";
            }
            int projeto_id;
            std::cout << "Digite o ID do projeto: ";
            if (!(std::cin >> projeto_id)) { limparEntrada(); continue; }
            limparEntrada();

            bool projetoValido = false;
            for (const auto& p : projetos) if (p.id == projeto_id) { projetoValido = true; break; }
            if (!projetoValido) {
                std::cout << "Projeto invalido.\n";
                continue;
            }

            auto avaliadores = Persist::carregarAvaliadores(dataDir() + "/avaliadores.csv");
            if (avaliadores.empty()) {
                std::cout << "Nao ha avaliadores cadastrados. Cadastre em (1).\n";
                continue;
            }
            std::cout << "\n=== Escolha o Avaliador ===\n";
            for (const auto& a : avaliadores) {
                std::cout << a.id << ") " << a.nome << " | " << a.email << " | " << a.area << "\n";
            }
            int avaliador_id;
            std::cout << "Digite o ID do avaliador: ";
            if (!(std::cin >> avaliador_id)) { limparEntrada(); continue; }
            limparEntrada();

            bool avaliadorValido = false;
            for (const auto& a : avaliadores) if (a.id == avaliador_id) { avaliadorValido = true; break; }
            if (!avaliadorValido) {
                std::cout << "Avaliador invalido.\n";
                continue;
            }

            double n1,n2,n3,n4;
            std::string comentario;
            std::cout << "Notas (4 numeros, 0-10): ";
            if (!(std::cin >> n1 >> n2 >> n3 >> n4)) { limparEntrada(); continue; }
            limparEntrada();
            std::cout << "Comentario: ";
            std::getline(std::cin, comentario);

            int id = api.registrarAvaliacao(projeto_id, avaliador_id, n1,n2,n3,n4, comentario);
            if (id != -1) std::cout << "Avaliacao registrada com ID " << id << "\n";
        }
        else if (op == 4) {
            auto r = api.ranking();
            if (r.empty()) {
                std::cout << "(sem avaliacoes)\n";
            } else {
                int pos = 1;
                for (auto& [media, pid] : r) {
                    std::cout << pos++ << "º - Projeto " << pid << " | media " << media << "\n";
                }
            }
        }
        else if (op == 5) {
            seed(api);
        }
        else if (op == 6) {
            api.exportarRankingCSV();
        }
        else if (op == 7) {
            auto projetos = Persist::carregarProjetos(dataDir() + "/projetos.csv");
            if (projetos.empty()) {
                std::cout << "Nao ha projetos cadastrados. Cadastre em (2).\n";
                continue;
            }

            std::cout << "\n=== Escolha o Projeto para ver detalhes ===\n";
            for (const auto& p : projetos) {
                std::cout << p.id << ") " << p.titulo << " | " << p.area << " | " << p.tipo << "\n";
            }

            int pid;
            std::cout << "Digite o ID do projeto: ";
            if (!(std::cin >> pid)) { limparEntrada(); continue; }
            limparEntrada();

            bool projetoValido = false;
            for (const auto& p : projetos) {
                if (p.id == pid) { projetoValido = true; break; }
            }
            if (!projetoValido) {
                std::cout << "Projeto invalido.\n";
                continue;
            }

            auto r = api.resumoProjeto(pid);
            std::cout << "Projeto " << pid << " - Avaliacoes: " << r.quantidade
                      << " | Media: " << r.media << "\n";

            if (r.quantidade == 0) {
                std::cout << "(sem comentarios)\n";
            } else {
                std::cout << "Comentarios:\n";
                int i = 1;
                for (auto& c : r.comentarios) {
                    std::cout << "  " << i++ << ") " << c << "\n";
                }
            }
        }
        else if (op == 8) {
            api.resetSistema();
        }
        else {
            std::cout << "Opcao invalida.\n";
        }
    }
    return 0;
}
