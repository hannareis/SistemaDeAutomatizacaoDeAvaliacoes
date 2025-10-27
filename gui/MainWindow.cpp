#include "MainWindow.h"

#include "RoleDialog.h"
#include "LoginDialog.h"
#include "DetalhesProjetoDialog.h"

#include <QMenuBar>
#include <QAction>
#include <QTabWidget>
#include <QToolBar>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QInputDialog>
#include <QMessageBox>

#include "../core/CoreAPI.h"
#include "../core/Persistencia.h"
#include "../core/Paths.h"

// -----------------------------------------------------
// CONSTRUTOR
// -----------------------------------------------------
MainWindow::MainWindow(UserRole role, QWidget* parent)
    : QMainWindow(parent), role_(role)
{
    setWindowTitle("AvaliaEncoeng");
    resize(980, 620);

    tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    // ===== Menu: "Sistema" → "Trocar Perfil…" =====
    auto* mSistema = menuBar()->addMenu("Sistema");
    auto* actTrocar = mSistema->addAction("Trocar Perfil…");
    actTrocar->setShortcut(QKeySequence("Ctrl+T"));

    connect(actTrocar, &QAction::triggered, this, [this]{
        RoleDialog dlg(this);
        if (dlg.exec() != QDialog::Accepted) return;

        auto escolhido = static_cast<UserRole>(dlg.roleSelecionado());
        if (escolhido == UserRole::Organizador) {
            // Pede senha APÓS escolher Organizador
            LoginDialog login(this);
            if (login.exec() == QDialog::Accepted && login.autenticado()) {
                setRole(UserRole::Organizador);
            } else {
                QMessageBox::information(this, "Acesso",
                                         "Acesso de organizador não liberado.");
                // mantém o perfil atual
            }
        } else {
            setRole(escolhido); // Avaliador/Público sem senha
        }
    });

    // ===== Toolbar fixa de perfis =====
    buildRoleToolbar();

    // ===== Abas iniciais =====
    if (role_ == UserRole::Organizador) {
        // Se o app abrir já como organizador, pedimos senha aqui
        LoginDialog login(this);
        if (login.exec() == QDialog::Accepted && login.autenticado()) {
            buildCadastrosTab(); // somente Cadastros
        } else {
            // se falhar, cai para Público
            role_ = UserRole::Publico;
            buildRankingTab();
        }
    } else if (role_ == UserRole::Avaliador) {
        buildAvaliarTab();
        buildRankingTab();
    } else {
        buildRankingTab();
    }
}

// -----------------------------------------------------
// TOOLBAR DE PERFIS (Organizador / Avaliador / Público)
// -----------------------------------------------------
void MainWindow::buildRoleToolbar() {
    QToolBar* tb = addToolBar("Perfis");
    tb->setMovable(false);

    QAction* actOrg  = tb->addAction("Organizador");
    QAction* actAval = tb->addAction("Avaliador");
    QAction* actPub  = tb->addAction("Público");

    // Organizador: pede senha imediatamente
    connect(actOrg, &QAction::triggered, this, [this]{
        LoginDialog login(this);
        if (login.exec() == QDialog::Accepted && login.autenticado()) {
            setRole(UserRole::Organizador);
        } else {
            QMessageBox::information(this, "Acesso",
                                     "Acesso de organizador não liberado.");
            // permanece no perfil atual
        }
    });

    // Avaliador: troca direto
    connect(actAval, &QAction::triggered, this, [this]{
        setRole(UserRole::Avaliador);
    });

    // Público: troca direto
    connect(actPub, &QAction::triggered, this, [this]{
        setRole(UserRole::Publico);
    });
}

// -----------------------------------------------------
// Troca de Perfil (reconstrói as abas conforme o perfil)
// -----------------------------------------------------
void MainWindow::setRole(UserRole role) {
    role_ = role;

    // Remove abas antigas
    while (tabs->count() > 0) {
        QWidget* w = tabs->widget(0);
        tabs->removeTab(0);
        w->deleteLater();
    }

    if (role_ == UserRole::Organizador) {
        // Já estamos validados aqui (senha foi pedida na ação)
        buildCadastrosTab(); // SOMENTE cadastros
    } else if (role_ == UserRole::Avaliador) {
        buildAvaliarTab();
        buildRankingTab();
    } else { // Público
        buildRankingTab();
    }
}

// -----------------------------------------------------
// ABA: CADASTROS (apenas no perfil ORGANIZADOR)
// -----------------------------------------------------
void MainWindow::buildCadastrosTab() {
    auto* page = new QWidget(this);
    auto* root = new QVBoxLayout(page);

    // ------ Avaliadores ------
    auto* gbA = new QGroupBox("Avaliadores", page);
    auto* fa  = new QFormLayout(gbA);

    auto* inNome      = new QLineEdit(gbA);
    auto* inEmail     = new QLineEdit(gbA);
    auto* inFormacao  = new QLineEdit(gbA);
    auto* inArea      = new QLineEdit(gbA);

    fa->addRow("Nome:",      inNome);
    fa->addRow("Email:",     inEmail);
    fa->addRow("Formação:",  inFormacao);
    fa->addRow("Área:",      inArea);

    auto* btnCadA  = new QPushButton("Cadastrar Avaliador", gbA);
    auto* btnListA = new QPushButton("Listar Avaliadores",  gbA);

    auto* rowA = new QHBoxLayout();
    rowA->addWidget(btnCadA);
    rowA->addWidget(btnListA);
    fa->addRow(rowA);

    // ------ Projetos ------
    auto* gbP = new QGroupBox("Projetos", page);
    auto* fp  = new QFormLayout(gbP);

    auto* inTitulo = new QLineEdit(gbP);
    auto* inAreaP  = new QLineEdit(gbP);
    auto* inTipo   = new QLineEdit(gbP);  // "PROJETO" ou "ARTIGO"

    fp->addRow("Título:", inTitulo);
    fp->addRow("Área:",   inAreaP);
    fp->addRow("Tipo:",   inTipo);

    auto* btnCadP  = new QPushButton("Cadastrar Projeto", gbP);
    auto* btnListP = new QPushButton("Listar Projetos",   gbP);

    auto* rowP = new QHBoxLayout();
    rowP->addWidget(btnCadP);
    rowP->addWidget(btnListP);
    fp->addRow(rowP);

    // ------ Tabela de listagem (reutilizada) ------
    auto* tabela = new QTableWidget(page);
    tabela->setColumnCount(4);
    tabela->setHorizontalHeaderLabels({"ID","Coluna 2","Coluna 3","Coluna 4"});
    tabela->horizontalHeader()->setStretchLastSection(true);

    // Montagem
    root->addWidget(gbA);
    root->addWidget(gbP);
    root->addWidget(tabela);
    root->addStretch(1);

    // ------ AÇÕES ------
    // Cadastrar Avaliador
    connect(btnCadA, &QPushButton::clicked, this, [=]{
        CoreAPI api; api.carregar();
        if (inNome->text().isEmpty() || inEmail->text().isEmpty()) {
            QMessageBox::warning(page, "Cadastro",
                                 "Nome e Email são obrigatórios.");
            return;
        }
        int id = api.cadastrarAvaliador(
            inNome->text().toStdString(),
            inEmail->text().toStdString(),
            inFormacao->text().toStdString(),
            inArea->text().toStdString()
        );
        QMessageBox::information(page, "Cadastro",
                                 QString("Avaliador cadastrado com ID %1").arg(id));
        inNome->clear(); inEmail->clear(); inFormacao->clear(); inArea->clear();
    });

    // Listar Avaliadores
    connect(btnListA, &QPushButton::clicked, this, [=]{
        auto lista = Persist::carregarAvaliadores(dataDir() + "/avaliadores.csv");
        tabela->clear();
        tabela->setColumnCount(4);
        tabela->setHorizontalHeaderLabels({"ID","Nome","Email","Área"});
        tabela->setRowCount((int)lista.size());
        for (int i=0;i<(int)lista.size();++i) {
            tabela->setItem(i,0,new QTableWidgetItem(QString::number(lista[i].id)));
            tabela->setItem(i,1,new QTableWidgetItem(QString::fromStdString(lista[i].nome)));
            tabela->setItem(i,2,new QTableWidgetItem(QString::fromStdString(lista[i].email)));
            tabela->setItem(i,3,new QTableWidgetItem(QString::fromStdString(lista[i].area)));
        }
    });

    // Cadastrar Projeto
    connect(btnCadP, &QPushButton::clicked, this, [=]{
        CoreAPI api; api.carregar();
        if (inTitulo->text().isEmpty() || inTipo->text().isEmpty()) {
            QMessageBox::warning(page, "Cadastro",
                                 "Título e Tipo são obrigatórios.");
            return;
        }
        int id = api.cadastrarProjeto(
            inTitulo->text().toStdString(),
            inAreaP->text().toStdString(),
            inTipo->text().toStdString()
        );
        QMessageBox::information(page, "Cadastro",
                                 QString("Projeto cadastrado com ID %1").arg(id));
        inTitulo->clear(); inAreaP->clear(); inTipo->clear();
    });

    // Listar Projetos
    connect(btnListP, &QPushButton::clicked, this, [=]{
        auto lista = Persist::carregarProjetos(dataDir() + "/projetos.csv");
        tabela->clear();
        tabela->setColumnCount(4);
        tabela->setHorizontalHeaderLabels({"ID","Título","Área","Tipo"});
        tabela->setRowCount((int)lista.size());
        for (int i=0;i<(int)lista.size();++i) {
            tabela->setItem(i,0,new QTableWidgetItem(QString::number(lista[i].id)));
            tabela->setItem(i,1,new QTableWidgetItem(QString::fromStdString(lista[i].titulo)));
            tabela->setItem(i,2,new QTableWidgetItem(QString::fromStdString(lista[i].area)));
            tabela->setItem(i,3,new QTableWidgetItem(QString::fromStdString(lista[i].tipo)));
        }
    });

    tabs->addTab(page, "Cadastros");
}

// -----------------------------------------------------
// ABA: AVALIAR (perfil Avaliador)
// -----------------------------------------------------
void MainWindow::buildAvaliarTab() {
    auto* page = new QWidget(this);
    auto* v = new QVBoxLayout(page);

    auto* title = new QLabel("Registrar Avaliação", page);
    title->setStyleSheet("font-size: 18px; font-weight: 600;");
    v->addWidget(title);

    // Seleção Projeto/Avaliador
    auto* rowSel = new QHBoxLayout();
    auto* lblProj = new QLabel("Projeto:", page);
    auto* cbProj  = new QComboBox(page);
    auto* lblAval = new QLabel("Avaliador:", page);
    auto* cbAval  = new QComboBox(page);
    rowSel->addWidget(lblProj); rowSel->addWidget(cbProj, 1);
    rowSel->addSpacing(12);
    rowSel->addWidget(lblAval); rowSel->addWidget(cbAval, 1);
    v->addLayout(rowSel);

    auto projetos = Persist::carregarProjetos(dataDir() + "/projetos.csv");
    for (const auto& p : projetos) {
        cbProj->addItem(QString::fromStdString(p.titulo) + " (" + QString::fromStdString(p.tipo) + ")",
                        QVariant(p.id));
    }
    auto avaliadores = Persist::carregarAvaliadores(dataDir() + "/avaliadores.csv");
    for (const auto& a : avaliadores) {
        cbAval->addItem(QString::fromStdString(a.nome) + " <" + QString::fromStdString(a.email) + ">",
                        QVariant(a.id));
    }

    // Notas
    auto* gridNotas = new QHBoxLayout();
    auto* n1 = new QDoubleSpinBox(page);
    auto* n2 = new QDoubleSpinBox(page);
    auto* n3 = new QDoubleSpinBox(page);
    auto* n4 = new QDoubleSpinBox(page);
    for (auto* sp : {n1,n2,n3,n4}) {
        sp->setRange(0.0, 10.0);
        sp->setSingleStep(0.5);
        sp->setDecimals(1);
        sp->setValue(8.0);
        gridNotas->addWidget(sp);
    }
    v->addWidget(new QLabel("Notas (0 a 10):", page));
    v->addLayout(gridNotas);

    // Comentário
    auto* txt = new QTextEdit(page);
    txt->setPlaceholderText("Comentário (mínimo 5 caracteres)...");
    txt->setFixedHeight(100);
    v->addWidget(txt);

    // Botão salvar
    auto* btnSalvar = new QPushButton("Registrar Avaliação", page);
    v->addWidget(btnSalvar);
    v->addStretch(1);

    connect(btnSalvar, &QPushButton::clicked, this, [=]{
        if (cbProj->count() == 0 || cbAval->count() == 0) {
            QMessageBox::warning(page, "Avaliação", "Cadastre Projeto e Avaliador primeiro.");
            return;
        }
        if (txt->toPlainText().trimmed().size() < 5) {
            QMessageBox::warning(page, "Avaliação", "Comentário muito curto.");
            return;
        }

        CoreAPI api; api.carregar();
        int projeto_id   = cbProj->currentData().toInt();
        int avaliador_id = cbAval->currentData().toInt();
        int novoId = api.registrarAvaliacao(
            projeto_id, avaliador_id,
            n1->value(), n2->value(), n3->value(), n4->value(),
            txt->toPlainText().toStdString()
        );

        if (novoId == -1) {
            QMessageBox::warning(page, "Avaliação",
                                 "Esse avaliador já avaliou este projeto.");
            return;
        }

        QMessageBox::information(page, "Avaliação",
                                 QString("Avaliação registrada com ID %1.").arg(novoId));
        txt->clear();
    });

    tabs->addTab(page, "Avaliar");
}

// -----------------------------------------------------
// ABA: RANKING (Avaliador e Público)
// -----------------------------------------------------
void MainWindow::buildRankingTab() {
    auto* page = new QWidget(this);
    auto* v = new QVBoxLayout(page);

    auto* title = new QLabel("Ranking de Projetos", page);
    title->setStyleSheet("font-size: 18px; font-weight: 600;");
    v->addWidget(title);

    // Tabela
    auto* table = new QTableWidget(page);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Posição", "Projeto ID", "Média"});
    table->horizontalHeader()->setStretchLastSection(true);
    v->addWidget(table);

    // Botões
    auto* btnRow = new QHBoxLayout();
    auto* btnAtualizar = new QPushButton("Atualizar Ranking", page);
    auto* btnExportar  = new QPushButton("Exportar CSV", page);
    auto* btnDetalhes  = new QPushButton("Ver Detalhes", page);
    btnRow->addWidget(btnAtualizar);
    btnRow->addWidget(btnDetalhes);
    btnRow->addWidget(btnExportar);
    v->addLayout(btnRow);

    // Preenche tabela
    auto preencherTabela = [=](){
        CoreAPI api; api.carregar();
        auto ranking = api.ranking();
        table->clearContents();
        table->setRowCount((int)ranking.size());
        int pos = 1;
        for (int i=0;i<(int)ranking.size();++i) {
            table->setItem(i,0,new QTableWidgetItem(QString::number(pos++) + "º"));
            table->setItem(i,1,new QTableWidgetItem(QString::number(ranking[i].second)));
            table->setItem(i,2,new QTableWidgetItem(QString::number(ranking[i].first,'f',2)));
        }
    };

    preencherTabela();
    connect(btnAtualizar, &QPushButton::clicked, this, preencherTabela);

    connect(btnExportar, &QPushButton::clicked, this, [=]{
        CoreAPI api; api.exportarRankingCSV();
        QMessageBox::information(page, "Exportação", "Ranking exportado com sucesso!");
    });

    connect(btnDetalhes, &QPushButton::clicked, this, [=]{
        bool ok = false;
        int pid = QInputDialog::getInt(page, "Detalhes do Projeto",
                                       "Digite o ID do projeto:", 1, 1, 999999, 1, &ok);
        if (!ok) return;
        DetalhesProjetoDialog dlg(pid, page);
        dlg.exec();
    });

    tabs->addTab(page, "Ranking");
}
