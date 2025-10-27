#include "RoleDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

RoleDialog::RoleDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Acessar AvaliaEncoeng");
    setModal(true);

    auto* root = new QVBoxLayout(this);

    auto* lbl = new QLabel("Escolha seu perfil:", this);
    root->addWidget(lbl);

    combo_ = new QComboBox(this);
    combo_->addItem("Organizador", QVariant(Organizador));
    combo_->addItem("Avaliador",   QVariant(Avaliador));
    combo_->addItem("Público",     QVariant(Publico));
    combo_->setCurrentIndex(2); // Público por padrão
    root->addWidget(combo_);

    auto* rowBtns = new QHBoxLayout();
    rowBtns->addStretch(1);
    auto* btnCancel = new QPushButton("Cancel", this);
    auto* btnOk     = new QPushButton("OK", this);
    rowBtns->addWidget(btnCancel);
    rowBtns->addWidget(btnOk);
    root->addLayout(rowBtns);

    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnOk,     &QPushButton::clicked, this, &QDialog::accept);
}

int RoleDialog::roleSelecionado() const {
    return combo_->currentData().toInt();
}
