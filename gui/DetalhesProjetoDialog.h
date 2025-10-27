#pragma once
#include <QDialog>
#include <vector>
#include <string>

class QLabel;
class QListWidget;

struct ResumoProjeto; // declarado no CoreAPI.h

class DetalhesProjetoDialog : public QDialog {
    Q_OBJECT
public:
    explicit DetalhesProjetoDialog(int projetoId, QWidget* parent = nullptr);

private:
    int projetoId_;
    QLabel* lblTitulo = nullptr;
    QLabel* lblResumo = nullptr;
    QListWidget* lstComentarios = nullptr;

    void carregarDados(); // chama CoreAPI::resumoProjeto
};
