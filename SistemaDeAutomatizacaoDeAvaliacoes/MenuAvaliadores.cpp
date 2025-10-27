#include <iostream>
#include <limits>
#include "MenuAvaliadores.hpp"
#include "Avaliador.hpp"
#include "TratamentoExcecao.hpp"
using namespace std;

void executarComTratamento(void (*func)()) {
    try { func(); }
    catch (const exception& e) { cout << "\n[Erro] " << e.what() << "\n"; }
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
        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
void menuCategoriaEArea(string& categoria, string& areaEspecialidade) {
    cout << "\nSelecione a Categoria:\n";
    cout << "1. Técnico\n";
    cout << "2. Graduação\n";
    cout << "Escolha: ";
    int opcCat;
    cin >> opcCat;
    cin.ignore();

    if (opcCat == 1) {
        categoria = "Técnico";
        cout << "\nCursos Técnicos disponíveis:\n";
        cout << "1. Automação Industrial\n";
        cout << "2. Eletrônica\n";
        cout << "3. Eletrotécnica\n";
        cout << "4. Informática (com Ênfase em Programação)\n";
        cout << "5. Mecânica\n";
        cout << "6. Mecatrônica\n";
        cout << "7. Qualidade\n";
        cout << "8. Logística\n";
        cout << "9. Segurança do Trabalho\n";
        cout << "Escolha o curso (1-9): ";
        int opcCurso;
        cin >> opcCurso;
        cin.ignore();

        switch (opcCurso) {
        case 1: areaEspecialidade = "Automação Industrial"; break;
        case 2: areaEspecialidade = "Eletrônica"; break;
        case 3: areaEspecialidade = "Eletrotécnica"; break;
        case 4: areaEspecialidade = "Informática (com Ênfase em Programação)"; break;
        case 5: areaEspecialidade = "Mecânica"; break;
        case 6: areaEspecialidade = "Mecatrônica"; break;
        case 7: areaEspecialidade = "Qualidade"; break;
        case 8: areaEspecialidade = "Logística"; break;
        case 9: areaEspecialidade = "Segurança do Trabalho"; break;
        default: areaEspecialidade = "Não definido"; break;
        }
    }
    else {
        categoria = "Graduação";
        cout << "\nCursos de Graduação disponíveis:\n";
        cout << "1. Administração\n";
        cout << "2. Ciência da Computação\n";
        cout << "3. Engenharia da Computação\n";
        cout << "4. Engenharia de Produção\n";
        cout << "5. Engenharia de Software\n";
        cout << "6. Engenharia Elétrica\n";
        cout << "7. Engenharia Mecânica\n";
        cout << "Escolha o curso (1-7): ";
        int opcCurso;
        cin >> opcCurso;
        cin.ignore();

        switch (opcCurso) {
        case 1: areaEspecialidade = "Administração"; break;
        case 2: areaEspecialidade = "Ciência da Computação"; break;
        case 3: areaEspecialidade = "Engenharia da Computação"; break;
        case 4: areaEspecialidade = "Engenharia de Produção"; break;
        case 5: areaEspecialidade = "Engenharia de Software"; break;
        case 6: areaEspecialidade = "Engenharia Elétrica"; break;
        case 7: areaEspecialidade = "Engenharia Mecânica"; break;
        default: areaEspecialidade = "Não definido"; break;
        }
    }

    cout << "\nCategoria e área selecionadas com sucesso!\n";
}
