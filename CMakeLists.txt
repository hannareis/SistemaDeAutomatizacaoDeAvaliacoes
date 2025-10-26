cmake_minimum_required(VERSION 3.16)
project(AvaliaEncoeng LANGUAGES CXX)

# Define a versão do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cria a biblioteca principal (core)
add_library(core
  core/Calculos.cpp
  core/Persistencia.cpp
  core/AvaliacaoModule.cpp
  core/MaxHeap.cpp
  core/Paths.cpp
  core/CoreAPI.cpp
)

# Garante que o cabeçalho (Calculos.h) seja encontrado
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR})

# Cria o executável principal (o programa que roda no terminal)
add_executable(avalia_console main_console.cpp)

# Liga o executável à biblioteca 'core'
target_link_libraries(avalia_console PRIVATE core)
