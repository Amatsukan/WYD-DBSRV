#!/bin/bash

# ============================================================================
# Script para Build do WYD-Server-CPP (Linux)
#
# Autor: Gemini
# Versao: 1.0
#
# Uso:
#   ./build.sh [comando]
#
# Comandos:
#   build        - (Padrao) Configura e compila o projeto no diretorio 'build'.
#   rebuild      - Limpa o diretorio de build e compila o projeto do zero.
#   clean        - Apaga o diretorio de build.
#   help, -h     - Mostra esta mensagem de ajuda.
#
# ============================================================================

# --- Configuracao ---
BUILD_DIR="build"

# --- Funcoes ---
show_help() {
    echo "Uso: ./build.sh [comando]"
    echo ""
    echo "Comandos:"
    echo "  build        - (Padrao) Configura e compila o projeto no diretorio 'build'."
    echo "  rebuild      - Limpa o diretorio de build e compila o projeto do zero."
    echo "  clean        - Apaga o diretorio de build."
    echo "  help, -h     - Mostra esta mensagem de ajuda."
}

do_build() {
    echo "--- Configurando o projeto com CMake (se necessario)... ---"
    if [ ! -d "$BUILD_DIR" ]; then
        mkdir "$BUILD_DIR"
    fi

    cmake -S . -B "$BUILD_DIR"
    if [ $? -ne 0 ]; then
        echo "Erro ao configurar o projeto com CMake."
        exit 1
    fi

    echo ""
    echo "--- Compilando o projeto... ---"
    cmake --build "$BUILD_DIR"
    if [ $? -ne 0 ]; then
        echo "Erro ao compilar o projeto."
        exit 1
    fi

    echo ""
    echo "Build concluido com sucesso!"
}

do_clean() {
    echo "--- Limpando o diretorio de build... ---"
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        echo "Diretorio '$BUILD_DIR' removido."
    else
        echo "Diretorio '$BUILD_DIR' nao encontrado. Nada a fazer."
    fi
}

# --- Logica do Script ---
COMMAND=$1
if [ -z "$COMMAND" ]; then
    COMMAND="build"
fi

echo "Acao: $COMMAND"
echo ""

case "$COMMAND" in
    build)
        do_build
        ;;
    rebuild)
        do_clean
        do_build
        ;;
    clean)
        do_clean
        ;;
    help|-h)
        show_help
        ;;
    *)
        echo "Comando invalido: '$COMMAND'. Use './build.sh help' para ver as opcoes."
        exit 1
        ;;
esac

exit 0