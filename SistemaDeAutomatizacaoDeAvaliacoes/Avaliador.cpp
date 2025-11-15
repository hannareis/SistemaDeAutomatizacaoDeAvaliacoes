#include "Avaliador.hpp"
#include "TratamentoExcecao.hpp"
#include "Menus.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <cctype>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::exception;
using std::getline;
using std::ios;
using std::numeric_limits;
using std::size_t;
using std::string;
using std::stringstream;
using std::vector;

// Caminho do arquivo (mantido)
const string ARQUIVO_AVALIADOR = "avaliadores.txt";

// ============================
// Helpers "privados" do arquivo
// ============================
namespace {
    // Nome: sem dígitos e com pelo menos 3 caracteres
    static bool contemDigito(const string& s) {
        for (unsigned char c : s) if (std::isdigit(c)) return true;
        return false;
    }

    static bool temLetra(const string& s) {
        for (unsigned char c : s) if (std::isalpha(c)) return true;
        return false;
    }

    // Ler linha com segurança (evita repetir ignore em vários pontos)
    static string getlineSafe() {
        string tmp;
        std::getline(cin, tmp);
        return tmp;
    }

    static bool emailValido(const std::string& e) {
        if (e.empty()) return false;
        size_t at = e.find('@');
        if (at == string::npos || at == 0 || at == e.size() - 1) return false;

        size_t dot = e.find('.', at + 1);
        if (dot == string::npos) return false;
        if (dot <= at + 1) return false;          // exige algo entre @ e .
        if (dot >= e.size() - 1) return false;    // não termina com ponto

        return true;
    }

}

// ============================
// Validações e Normalizações
// ============================
bool validarCPF(const string& cpf) {
    string apenasDigitos;
    for (char c : cpf)
        if (std::isdigit(static_cast<unsigned char>(c))) apenasDigitos += c;

    if (apenasDigitos.length() != 11) return false;
    if (apenasDigitos == string(11, apenasDigitos[0])) return false;

    int soma = 0, resto;
    for (int i = 0; i < 9; i++)
        soma += (apenasDigitos[i] - '0') * (10 - i);
    resto = (soma * 10) % 11;
    if (resto == 10) resto = 0;
    if (resto != (apenasDigitos[9] - '0')) return false;

    soma = 0;
    for (int i = 0; i < 10; i++)
        soma += (apenasDigitos[i] - '0') * (11 - i);
    resto = (soma * 10) % 11;
    if (resto == 10) resto = 0;
    if (resto != (apenasDigitos[10] - '0')) return false;

    return true;
}

string limparCPF(const string& cpf) {
    string apenasDigitos;
    for (char c : cpf)
        if (std::isdigit(static_cast<unsigned char>(c))) apenasDigitos += c;
    return apenasDigitos;
}

// ============================
// Persistência (arquivo)
// ============================
vector<Avaliador> carregarAvaliadores() {
    vector<Avaliador> lista;
    std::ifstream file(ARQUIVO_AVALIADOR);
    if (!file.is_open()) return lista;

    string linha;
    while (std::getline(file, linha)) {
        if (linha.empty()) continue;
        Avaliador a;
        stringstream ss(linha);
        std::getline(ss, a.cpf, ';');
        std::getline(ss, a.nome, ';');
        std::getline(ss, a.email, ';');
        std::getline(ss, a.senha, ';');
        std::getline(ss, a.categoria, ';');
        std::getline(ss, a.areaEspecialidade, ';');
        lista.push_back(a);
    }
    return lista;
}

void salvarAvaliadores(const vector<Avaliador>& lista) {
    std::ofstream file(ARQUIVO_AVALIADOR, ios::trunc);
    if (!file.is_open()) {
        cout << "[Erro] Não foi possível abrir o arquivo '" << ARQUIVO_AVALIADOR << "' para escrita (truncate).\n";
        return;
    }
    for (const auto& a : lista) {
        file << limparCPF(a.cpf) << ';'
            << a.nome << ';'
            << a.email << ';'
            << a.senha << ';'
            << a.categoria << ';'
            << a.areaEspecialidade << '\n';
    }
}

// ============================
// Consultas / Regras simples
// ============================
static bool verificarRecursivo(const vector<Avaliador>& lista, const string& cpf, int i) {
    if (i >= static_cast<int>(lista.size())) return false;
    const string cpfLista = limparCPF(lista[i].cpf);
    const string cpfDigitado = limparCPF(cpf);
    if (cpfLista == cpfDigitado) return true;
    return verificarRecursivo(lista, cpf, i + 1);
}

static bool verificarRecursivoSenha(const vector<Avaliador>& lista, const string& senha, int i) {
    if (i >= static_cast<int>(lista.size())) return false;
    const string cpfLista = limparCPF(lista[i].senha);
    const string cpfDigitado = limparCPF(senha);
    if (cpfLista == cpfDigitado) return true;
    return verificarRecursivoSenha(lista, senha, i + 1);
}

bool verificarCPFExistente(const string& cpf) {
    auto lista = carregarAvaliadores();
    return verificarRecursivo(lista, cpf, 0);
}

bool verificarSenhaExistente(const std::string& senha) {
    auto lista = carregarAvaliadores();
    return verificarRecursivo(lista, senha, 0);
}

bool verificarEmailExistente(const string& email) {
    auto lista = carregarAvaliadores();
    for (const auto& a : lista)
        if (a.email == email) return true;
    return false;
}

// ============================
// Fluxos (UI) / Operações
// ============================
void criarAvaliador() {
    Avaliador a;
    cout << "\n=== Bem-vindo ao Cadastro do Avaliador ===\n";

    enum Etapa { NOME = 0, EMAIL, CPF, SENHA, CATEGORIA, SALVAR };
    Etapa etapa = NOME;

    while (true) {
        switch (etapa) {
        case NOME: {
            cout << "Digite o nome (ou 0 para cancelar): ";
            a.nome = getlineSafe();

            if (a.nome == "0") {
                cout << "Cancelando o cadastro e voltando ao menu...\n";
                return;
            }
            if (a.nome.empty() || a.nome.size() < 3 || contemDigito(a.nome) || !temLetra(a.nome)) {
                cout << "[Erro] Use apenas letras e no mínimo 3 caracteres.\n";
                break; // permanece em NOME
            }
            etapa = EMAIL;
            break;
        }

        case EMAIL: {
            cout << "Digite o e-mail (ou 0 para voltar): ";
            a.email = getlineSafe();

            if (a.email == "0") {
                cout << "Voltando para o campo Nome...\n";
                etapa = NOME;
                break;
            }

            try {
                if (a.email.empty())
                    throw ExcecaoCampoVazio("O e-mail não pode estar em branco. Informe um e-mail válido.");

                // validação simples: nome@dominio.com
                size_t posArroba = a.email.find('@');
                size_t posPonto = a.email.find('.', (posArroba == string::npos ? 0 : posArroba + 1));
                if (posArroba == string::npos || posPonto == string::npos ||
                    posPonto <= posArroba + 1 || posPonto >= a.email.size() - 1)
                    throw ExcecaoAutenticacao("Formato de e-mail inválido. Use algo como nome@dominio.com.");

                if (verificarEmailExistente(a.email))
                    throw ExcecaoAutenticacao("Este e-mail já está em uso. Tente outro.");

                etapa = CPF;
            }
            catch (const exception& e) {
                cout << "[Erro de E-mail] " << e.what() << "\nTente novamente.\n";
            }
            break;
        }

        case CPF: {
            cout << "Digite o CPF (com ou sem pontos) (ou 0 para voltar): ";
            string entradaCPF = getlineSafe();

            if (entradaCPF == "0") {
                cout << "Voltando para o campo E-mail...\n";
                etapa = EMAIL;
                break;
            }

            try {
                // trabalha SEMPRE com CPF limpo
                string cpfL = limparCPF(entradaCPF);

                if (!validarCPF(cpfL))
                    throw ExcecaoCPFInvalido("CPF inválido. Verifique e tente novamente.");

                if (verificarCPFExistente(cpfL))
                    throw ExcecaoAutenticacao("Este CPF já está cadastrado.");

                a.cpf = cpfL;  // guarda limpo para persistir consistente
                etapa = SENHA;
            }
            catch (const exception& e) {
                cout << "[Erro de CPF] " << e.what() << "\nTente novamente.\n";
            }
            break;
        }

        case SENHA: {
            cout << "Crie uma senha (ou 0 para voltar): ";
            string entradaSenha = getlineSafe();

            if (entradaSenha == "0") {
                cout << "Voltando para o campo CPF...\n";
                etapa = CPF;
                break;
            }
            if (entradaSenha.empty()) {
                cout << "[Erro de Senha] A senha não pode estar vazia.\n";
                break; // permanece em SENHA
            }
            a.senha = entradaSenha;
            etapa = CATEGORIA;
            break;
        }

        case CATEGORIA: {
            // se o usuário escolher 0 no submenu, volte apenas para a etapa anterior (SENHA)
            if (!menuCategoriaEArea(a.categoria, a.areaEspecialidade)) {
                cout << "Voltando para a etapa Senha...\n";
                etapa = SENHA;   // <-- em vez de retornar, volta 1 etapa
                break;
            }
            etapa = SALVAR;      // confirmou categoria e curso
            break;
        }

        case SALVAR: {
            std::ofstream file(ARQUIVO_AVALIADOR, ios::app);
            if (!file.is_open()) {
                cout << "[Erro] Não foi possível abrir o arquivo '" << ARQUIVO_AVALIADOR << "' para escrita (append).\n";
                return;
            }
            file << limparCPF(a.cpf) << ';'
                << a.nome << ';'
                << a.email << ';'
                << a.senha << ';'
                << a.categoria << ';'
                << a.areaEspecialidade << '\n';
            file.close();

            cout << "\nAvaliador cadastrado com sucesso!\n";
            return;
        }
        } // switch
    } // while
}


void listarAvaliadores() {
    auto lista = carregarAvaliadores();
    if (lista.empty()) {
        cout << "\nNenhum avaliador cadastrado.\n";
        return;
    }

    cout << "\n=== Lista de Avaliadores ===\n";
    for (const auto& a : lista) {
        cout << "\nCPF: " << a.cpf
            << "\nNome: " << a.nome
            << "\nE-mail: " << a.email
            << "\nCategoria: " << a.categoria
            << "\nÁrea: " << a.areaEspecialidade << "\n";
    }
}

void atualizarAvaliador() {
    auto lista = carregarAvaliadores();
    cout << "\n=== Atualizar Avaliador ===\nCPF: ";
    string cpf = getlineSafe();

    string cpfDigitado = limparCPF(cpf);
    bool achou = false;

    for (auto& a : lista) {
        if (limparCPF(a.cpf) == cpfDigitado) {
            achou = true;
            int opcao;

            do {
                cout << "\nO que deseja atualizar?\n"
                    << "1. E-mail\n2. Senha\n3. Categoria e Área\n0. Voltar\nEscolha: ";
                if (!(cin >> opcao)) {
                    cin.clear();
                    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                    opcao = 0;
                }
                cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

                switch (opcao) {
                case 1: {
                    cout << "Novo e-mail: ";
                    string novoEmail = getlineSafe();

                    // vazio?
                    if (novoEmail.empty()) {
                        cout << "[Erro] O e-mail não pode ficar em branco.\n";
                        break;
                    }

                    // não mudou?
                    if (novoEmail == a.email) {
                        cout << "E-mail não mudou.\n";
                        break;
                    }

                    // formato válido?
                    if (!emailValido(novoEmail)) {
                        cout << "[Erro] Formato inválido. Use algo como nome@dominio.com.\n";
                        break;
                    }

                    // duplicidade (ignora o próprio CPF)
                    bool emUsoPorOutro = false;
                    for (const auto& outro : lista) {
                        if (limparCPF(outro.cpf) == limparCPF(a.cpf)) continue; // é o mesmo registro
                        if (outro.email == novoEmail) { emUsoPorOutro = true; break; }
                    }
                    if (emUsoPorOutro) {
                        cout << "E-mail já cadastrado por outro usuário!\n";
                        break;
                    }

                    // OK, atualiza
                    a.email = novoEmail;
                    cout << "E-mail atualizado!\n";
                    break;
                }
                case 2: {
                    cout << "Nova senha: ";
                    string novaSenha = getlineSafe();
                    if (!novaSenha.empty()) {
                        a.senha = novaSenha;
                        cout << "Senha atualizada!\n";
                    }
                    break;
                }
                case 3: {
                    if (!menuCategoriaEArea(a.categoria, a.areaEspecialidade)) {
                        cout << "Voltando...\n";
                        break; // não imprime 'atualizadas' se o usuário voltou
                    }
                    cout << "Categoria e área atualizadas!\n";
                    break;
                } 

                case 0:
                    cout << "Voltando...\n";
                    break;
                default:
                    cout << "Opção inválida.\n";
                }
            } while (opcao != 0);
            break;
        }
    }

    if (achou) {
        salvarAvaliadores(lista);
        cout << "\nAvaliador atualizado com sucesso!\n";
    }
    else {
        cout << "\nCPF não encontrado.\n";
    }
}

void deletarAvaliador() {
    auto lista = carregarAvaliadores();
    cout << "\n=== Deletar Avaliador ===\nCPF: ";
    string cpf = getlineSafe();

    string cpfDigitado = limparCPF(cpf);
    vector<Avaliador> novaLista;
    Avaliador alvo;
    bool achou = false;

    for (auto& a : lista) {
        if (limparCPF(a.cpf) == cpfDigitado) {
            achou = true;
            alvo = a;
        }
        else {
            novaLista.push_back(a);
        }
    }

    if (!achou) {
        cout << "\nCPF não encontrado.\n";
        return;
    }

    cout << "\nAvaliador encontrado:\n";
    cout << "Nome: " << alvo.nome << "\nÁrea: " << alvo.areaEspecialidade << "\n";
    cout << "\nTem certeza que deseja excluir?\n1. Sim\n2. Não\nEscolha: ";

    int op;
    if (!(cin >> op)) {
        cin.clear();
        op = 2;
    }
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

    if (op == 1) {
        salvarAvaliadores(novaLista);
        cout << "\nAvaliador removido com sucesso!\n";
    }
    else {
        cout << "\nOperação cancelada.\n";
    }
}

// ============================
// Autenticação / Login
// ============================
bool autenticarAvaliador(const string& cpf, const string& senha) {
    auto lista = carregarAvaliadores();
    string cpfDigitado = limparCPF(cpf);

    for (const auto& a : lista) {
        if (limparCPF(a.cpf) == cpfDigitado) {
            if (a.senha == senha) return true;
            throw ExcecaoSenhaIncorreta("Senha incorreta!");
        }
    }
    throw ExcecaoAutenticacao("CPF não encontrado!");
}

void loginAvaliador() {
    cout << "\n=== Login de Avaliador ===\n";
    cout << "CPF: ";
    string cpf = getlineSafe();
    cout << "Senha: ";
    string senha = getlineSafe();

    try {
        if (autenticarAvaliador(cpf, senha))
            cout << "\n✔ Login realizado com sucesso!\n";
    }
    catch (const exception& e) {
        cout << "[Erro de Login] " << e.what() << "\n";
    }
}
