#pragma once
#include <QDialog>

class QComboBox;

class RoleDialog : public QDialog {
    Q_OBJECT
public:
    enum Role { Organizador = 0, Avaliador = 1, Publico = 2 };

    explicit RoleDialog(QWidget* parent = nullptr);
    int roleSelecionado() const;

private:
    QComboBox* combo_ = nullptr;
};
