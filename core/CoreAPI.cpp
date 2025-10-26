#include <filesystem> // <-- necessário para criar a pasta data/ se não existir
#include <fstream>   // <- necessário para std::ofstream
#include <iostream>
#include "CoreAPI.h"
#include "Persistencia.h"
#include "Calculos.h"
#include "Paths.h"
#include <algorithm>

void CoreAPI::carregar() {
    // Hoje não mantém cache; leitura é on-demand em cada método.
}

// ---------- Helpers de caminho + IO ----------
std::vector<Avaliador> CoreAPI::loadAvaliadores() {
    return Persist::carregarAvaliadores(dataDir() + "/avaliadores.csv");
}
std::vector<Projeto> CoreAPI::loadProjetos() {
    return Persist::carregarProjetos(dataDir() + "/projetos.csv");
}
std::vector<Avaliacao> CoreAPI::loadAvaliacoes() {
    return Persist::carregarAvaliacoes(dataDir() + "/avaliacoes.csv");
}
void CoreAPI::saveAvaliadores(const std::vector<Avaliador>& v) {
    Persist::salvarAvaliadores(dataDir() + "/avaliadores.csv", v);
}
void CoreAPI::saveProjetos(const std::vector<Projeto>& v) {
    Persist::salvarProjetos(dataDir() + "/projetos.csv", v);
}
void CoreAPI::saveAvaliacoes(const std::vector<Avaliacao>& v) {
    Persist::salvarAvaliacoes(dataDir() + "/avaliacoes.csv", v);
}

// ---------- Próximo ID (max + 1) ----------
int CoreAPI::proximoIDAvaliador() {
    auto v = loadAvaliadores();
    int maxid = 0; for (auto& a : v) maxid = std::max(maxid, a.id);
    return maxid + 1;
}
int CoreAPI::proximoIDProjeto() {
    auto v = loadProjetos();
    int maxid = 0; for (auto& p : v) maxid = std::max(maxid, p.id);
    return maxid + 1;
}
int CoreAPI::proximoIDAvaliacao() {
    auto v = loadAvaliacoes();
    int maxid = 0; for (auto& a : v) maxid = std::max(maxid, a.id);
    return maxid + 1;
}

// ---------- Cadastros ----------
int CoreAPI::cadastrarAvaliador(const std::string& nome, const std::string& email,
                                const std::string& formacao, const std::string& area) {
    auto lista = loadAvaliadores();
    Avaliador a{proximoIDAvaliador(), nome, email, formacao, area};
    lista.push_back(a);
    saveAvaliadores(lista);
    return a.id;
}

int CoreAPI::cadastrarProjeto(const std::string& titulo, const std::string& area,
                              const std::string& tipo) {
    auto lista = loadProjetos();

    // normaliza TIPO para maiúsculas (mantém só a palavra-chave PROJETO/ARTIGO)
    std::string tipoNorm;
    // regra simples: se a string contiver "ARTIGO" (ignorando maiúsc/minúsc), vira ARTIGO; senão PROJETO
    for (char c : tipo) tipoNorm.push_back(std::toupper(static_cast<unsigned char>(c)));
    if (tipoNorm.find("ARTIGO") != std::string::npos) tipoNorm = "ARTIGO";
    else tipoNorm = "PROJETO";

    Projeto p{proximoIDProjeto(), titulo, area, tipoNorm};
    lista.push_back(p);
    saveProjetos(lista);
    return p.id;
}

// ---------- Registrar avaliação ----------
int CoreAPI::registrarAvaliacao(int projeto_id, int avaliador_id,
                                double n1, double n2, double n3, double n4,
                                const std::string& comentario) {
    auto lista = loadAvaliacoes();

    // --- Nova validação: impedir avaliação duplicada ---
    for (auto& av : lista) {
    	if (av.projeto_id == projeto_id && av.avaliador_id == avaliador_id) {
        	std::cout << "⚠️  Esse avaliador já avaliou este projeto!\n";
        	return -1;
    	}
    }

    std::vector<double> notas = {n1,n2,n3,n4};
    double media = mediaRecursiva(notas);

    Avaliacao av;
    av.id = proximoIDAvaliacao();
    av.projeto_id = projeto_id;
    av.avaliador_id = avaliador_id;
    av.n1 = n1; av.n2 = n2; av.n3 = n3; av.n4 = n4;
    av.comentario = comentario;
    av.media = media;

    lista.push_back(av);
    saveAvaliacoes(lista);
    return av.id;
}

// ---------- Ranking ----------
std::vector<std::pair<double,int>> CoreAPI::ranking() {
    auto avals = loadAvaliacoes();
    // média das médias por projeto
    std::map<int, std::vector<double>> porProjeto;
    for (auto& av : avals) porProjeto[av.projeto_id].push_back(av.media);

    std::vector<std::pair<double,int>> r;
    r.reserve(porProjeto.size());
    for (auto& [pid, arr] : porProjeto) {
        double soma = 0.0;
        for (double m : arr) soma += m;
        double med = arr.empty() ? 0.0 : soma / arr.size();
        r.push_back({med, pid});
    }
    std::sort(r.begin(), r.end(), [](auto& a, auto& b){
        if (a.first != b.first) return a.first > b.first; // maior média primeiro
        return a.second < b.second; // desempate por id menor
    });
    return r;
}

// ---------- Exportar ranking em CSV ----------
void CoreAPI::exportarRankingCSV() {
    auto r = ranking();
    if (r.empty()) {
        std::cout << "⚠️  Nenhuma avaliacao encontrada. Nada para exportar.\n";
        return;
    }

    std::ofstream arq(dataDir() + "/ranking.csv");
    arq << "posicao;projeto_id;media\n";
    int pos = 1;
    for (auto& [media, pid] : r) {
        arq << pos++ << ";" << pid << ";" << media << "\n";
    }
    arq.close();

    std::cout << "✅  Ranking exportado para: " << dataDir() + "/ranking.csv" << "\n";
}

// ---------- Detalhes do Projeto ----------
ResumoProjeto CoreAPI::resumoProjeto(int projeto_id) {
    auto avals = loadAvaliacoes();
    ResumoProjeto r{0.0, 0, {}};

    double soma = 0.0;
    for (auto& av : avals) {
        if (av.projeto_id == projeto_id) {
            soma += av.media;
            r.quantidade++;
            r.comentarios.push_back(av.comentario);
        }
    }
    r.media = (r.quantidade > 0) ? (soma / r.quantidade) : 0.0;
    return r;
}

// ---------- Resetar os Dados do Programa ----------
void CoreAPI::resetSistema() {
    namespace fs = std::filesystem;
    const std::string dir = dataDir();

    if (!fs::exists(dir)) {
        fs::create_directories(dir);
    }

    // Recria arquivos com cabeçalhos
    std::ofstream(dir + "/avaliadores.csv") 
        << "id;nome;email;formacao;area\n";
    std::ofstream(dir + "/projetos.csv") 
        << "id;titulo;area;tipo\n";
    std::ofstream(dir + "/avaliacoes.csv") 
        << "id;projeto_id;avaliador_id;n1;n2;n3;n4;comentario;media_calculada\n";
    std::ofstream(dir + "/ranking.csv") 
        << "posicao;projeto_id;media\n";

    std::cout << "⚙️  Sistema resetado: CSVs recriados em '" << dir << "'.\n";
}
