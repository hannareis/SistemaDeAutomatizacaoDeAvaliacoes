#ifndef MENU_AVALIADORES_HPP
#define MENU_AVALIADORES_HPP

#include <string>

// Menu CRUD de avaliadores (abre o submenu com criar/listar/atualizar/deletar)
void menuAvaliadores();

// SubMenu para escolher Categoria e Área (preenche por referência dentro do menu Avaliadores)
bool menuCategoriaEArea(std::string& categoria, std::string& areaEspecialidade);

#endif 