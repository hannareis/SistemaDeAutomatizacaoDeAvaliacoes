#include <QApplication>
#include "MainWindow.h"
#include "RoleDialog.h"
#include "LoginDialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Escolhe o perfil inicial
    UserRole selected = UserRole::Publico;
    {
        RoleDialog dlg;
        if (dlg.exec() == QDialog::Accepted) {
            auto chosen = static_cast<UserRole>(dlg.roleSelecionado());
            if (chosen == UserRole::Organizador) {
                // Pede a senha APÓS escolher organizador
                LoginDialog login;
                if (login.exec() == QDialog::Accepted && login.autenticado()) {
                    selected = UserRole::Organizador;
                } else {
                    // Falhou a senha => inicia em Público
                    selected = UserRole::Publico;
                }
            } else {
                // Avaliador ou Público sem senha
                selected = chosen;
            }
        } else {
            // Cancelou o diálogo => inicia em Público
            selected = UserRole::Publico;
        }
    }

    MainWindow win(selected);
    win.show();
    return app.exec();
}
