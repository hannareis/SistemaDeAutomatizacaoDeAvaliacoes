cmake_minimum_required(VERSION 3.16)
project(AvaliaEncoeng LANGUAGES CXX)

# Versão do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilita AUTOMOC para classes Qt com Q_OBJECT
set(CMAKE_AUTOMOC ON)

# ----------------- Biblioteca principal (core) -----------------
add_library(core
  core/Calculos.cpp
  core/Persistencia.cpp
  core/AvaliacaoModule.cpp
  core/MaxHeap.cpp
  core/Paths.cpp
  core/CoreAPI.cpp
)

# Inclui cabeçalhos do projeto
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR})

# ----------------- Executável de console -----------------
add_executable(avalia_console main_console.cpp)
target_link_libraries(avalia_console PRIVATE core)

# ----------------- Interface gráfica Qt -----------------
# Ajuste o prefixo caso seu brew --prefix qt seja diferente
set(QT_PREFIX "/opt/homebrew/opt/qt")
set(CMAKE_PREFIX_PATH "${QT_PREFIX}")
find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(avalia_gui
    gui/main_qt.cpp
    gui/MainWindow.cpp
    gui/MainWindow.h
    gui/DetalhesProjetoDialog.cpp
    gui/DetalhesProjetoDialog.h
    gui/LoginDialog.cpp
    gui/LoginDialog.h
    gui/RoleDialog.cpp
    gui/RoleDialog.h
)

target_link_libraries(avalia_gui PRIVATE
    Qt6::Widgets
    core
)
