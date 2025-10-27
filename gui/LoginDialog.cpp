#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QMessageBox>

static const char* ADMIN_PASS = "admin123";

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Login do Organizador");
    resize(300, 150);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Digite a senha de administrador:", this));

    inputSenha = new QLineEdit(this);
    inputSenha->setEchoMode(QLineEdit::Password);
    layout->addWidget(inputSenha);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, [=] {
        if (inputSenha->text() == ADMIN_PASS) {
            ok_ = true;
            accept();
        } else {
            QMessageBox::warning(this, "Erro", "Senha incorreta.");
        }
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
