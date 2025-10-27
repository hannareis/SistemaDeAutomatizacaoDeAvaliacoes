#include "DetalhesProjetoDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QString>

#include "../core/CoreAPI.h"

DetalhesProjetoDialog::DetalhesProjetoDialog(int projetoId, QWidget* parent)
    : QDialog(parent), projetoId_(projetoId)
{
    setWindowTitle("Detalhes do Projeto");
    resize(500, 400);

    auto* root = new QVBoxLayout(this);

    lblTitulo = new QLabel(QString("Projeto ID: %1").arg(projetoId_), this);
    lblTitulo->setStyleSheet("font-size: 16px; font-weight: 600;");
    root->addWidget(lblTitulo);

    lblResumo = new QLabel("Carregando média e comentários...", this);
    root->addWidget(lblResumo);

    lstComentarios = new QListWidget(this);
    root->addWidget(lstComentarios, 1);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    root->addWidget(buttons);

    carregarDados();
}

void DetalhesProjetoDialog::carregarDados() {
    CoreAPI api;
    api.carregar();
    ResumoProjeto r = api.resumoProjeto(projetoId_);

    lblResumo->setText(
        QString("Avaliações: %1   |   Média: %2")
            .arg(r.quantidade)
            .arg(QString::number(r.media, 'f', 2))
    );

    lstComentarios->clear();
    if (r.comentarios.empty()) {
        lstComentarios->addItem("(sem comentários)");
    } else {
        int i = 1;
        for (const auto& c : r.comentarios) {
            lstComentarios->addItem(QString::number(i++) + ") " + QString::fromStdString(c));
        }
    }
}
