#pragma once
#include <QDialog>

class QLineEdit;

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget* parent = nullptr);
    bool autenticado() const { return ok_; }

private:
    bool ok_ = false;
    QLineEdit* inputSenha = nullptr;
};
