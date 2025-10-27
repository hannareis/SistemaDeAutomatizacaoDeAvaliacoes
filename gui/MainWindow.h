#pragma once
#include <QMainWindow>

class QTabWidget;

// Perfil de uso da janela
enum class UserRole { Organizador = 0, Avaliador = 1, Publico = 2 };

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(UserRole role = UserRole::Publico, QWidget* parent = nullptr);

private:
    QTabWidget* tabs = nullptr;
    UserRole role_ = UserRole::Publico;

    // Construção da UI
    void buildRoleToolbar();
    void buildCadastrosTab(); // só no perfil Organizador
    void buildAvaliarTab();   // Avaliador
    void buildRankingTab();   // Avaliador + Público

    // Troca de perfil (recria as abas)
    void setRole(UserRole role);
};
