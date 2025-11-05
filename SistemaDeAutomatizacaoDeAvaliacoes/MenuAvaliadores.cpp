#include <iostream>
#include <limits>
#include "Menus.hpp"
#include "Avaliador.hpp"
#include "TratamentoExcecao.hpp"

using std::cin;
using std::cout;
using std::string;

// Wrapper local (não exposto no header)
namespace {
    void executarComTratamento(void (*func)()) {
        try { func(); }
        catch (const std::exception& e) { cout << "\n[Erro] " << e.what() << "\n"; }
    }

    // helper pra ler inteiro com validação
    bool lerInteiro(int& out) {
        cin >> out;
        if (!cin) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return true;
    }
}

void menuAvaliadores() {
    int opcao;
    do {
        cout << "\n=== Menu de Avaliadores ===\n";
        cout << "1. Cadastrar Avaliador\n";
        cout << "2. Listar Avaliadores\n";
        cout << "3. Atualizar Avaliador\n";
        cout << "4. Deletar Avaliador\n";
        cout << "0. Voltar\n";
        cout << "Escolha: ";

        if (!lerInteiro(opcao)) {
            cout << "Entrada inválida!\n";
            opcao = -1;
        }

        switch (opcao) {
        case 1: executarComTratamento(criarAvaliador); break;
        case 2: listarAvaliadores(); break;
        case 3: executarComTratamento(atualizarAvaliador); break;
        case 4: executarComTratamento(deletarAvaliador); break;
        case 0: cout << "Voltando ao menu principal...\n"; break;
        default: cout << "Opção inválida!\n";
        }
    } while (opcao != 0);
}

bool menuCategoriaEArea(std::string& categoria, std::string& areaEspecialidade) {
    while (true) { // LOOP EXTERNO: escolher a CATEGORIA
        int opcCat;
        std::cout << "\nSelecione a Categoria:\n"
            << "1. Técnico\n"
            << "2. Graduação\n"
            << "0. Voltar\n"
            << "Escolha: ";
        if (!(std::cin >> opcCat)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada inválida.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcCat == 0) return false;              // ? volta para etapa anterior (SENHA)
        if (opcCat != 1 && opcCat != 2) { std::cout << "Opção inválida.\n"; continue; }

        if (opcCat == 1) {
            categoria = "Técnico";
            while (true) { // LOOP INTERNO: escolher CURSO TÉCNICO
                int opc;
                std::cout << "\nCursos Técnicos disponíveis:\n"
                    << "1. Automação Industrial\n2. Eletrônica\n3. Eletrotécnica\n"
                    << "4. Informática (com Ênfase em Programação)\n5. Mecânica\n6. Mecatrônica\n"
                    << "7. Qualidade\n8. Logística\n9. Segurança do Trabalho\n"
                    << "0. Voltar\nEscolha o curso (0–9): ";
                if (!(std::cin >> opc)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Entrada inválida.\n"; continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (opc == 0) break; // ? volta para ESCOLHER A CATEGORIA (loop externo)

                switch (opc) {
                case 1: areaEspecialidade = "Automação Industrial"; break;
                case 2: areaEspecialidade = "Eletrônica"; break;
                case 3: areaEspecialidade = "Eletrotécnica"; break;
                case 4: areaEspecialidade = "Informática (com Ênfase em Programação)"; break;
                case 5: areaEspecialidade = "Mecânica"; break;
                case 6: areaEspecialidade = "Mecatrônica"; break;
                case 7: areaEspecialidade = "Qualidade"; break;
                case 8: areaEspecialidade = "Logística"; break;
                case 9: areaEspecialidade = "Segurança do Trabalho"; break;
                default: std::cout << "Opção inválida.\n"; continue;
                }
                std::cout << "\nCategoria e área selecionadas com sucesso!\n";
                return true; // confirmou
            }
            // 0 no curso técnico ? volta para selecionar categoria
        }
        else {
            categoria = "Graduação";
            while (true) { // LOOP INTERNO: escolher CURSO DE GRADUAÇÃO
                int opc;
                std::cout << "\nCursos de Graduação disponíveis:\n"
                    << "1. Administração\n2. Ciência da Computação\n3. Engenharia da Computação\n"
                    << "4. Engenharia de Produção\n5. Engenharia de Software\n6. Engenharia Elétrica\n"
                    << "7. Engenharia Mecânica\n0. Voltar\nEscolha o curso (0–7): ";
                if (!(std::cin >> opc)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Entrada inválida.\n"; continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (opc == 0) break; // ? volta para ESCOLHER A CATEGORIA (loop externo)

                switch (opc) {
                case 1: areaEspecialidade = "Administração"; break;
                case 2: areaEspecialidade = "Ciência da Computação"; break;
                case 3: areaEspecialidade = "Engenharia da Computação"; break;
                case 4: areaEspecialidade = "Engenharia de Produção"; break;
                case 5: areaEspecialidade = "Engenharia de Software"; break;
                case 6: areaEspecialidade = "Engenharia Elétrica"; break;
                case 7: areaEspecialidade = "Engenharia Mecânica"; break;
                default: std::cout << "Opção inválida.\n"; continue;
                }
                std::cout << "\nCategoria e área selecionadas com sucesso!\n";
                return true; // confirmou
            }
            // 0 no curso de graduação ? volta para selecionar categoria
        }
    }
}
