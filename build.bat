@echo off
setlocal

REM ============================================================================
REM Script para Build do WYD-Server-CPP
REM
REM Autor: Gemini
REM Versao: 1.0
REM
REM Este script deve ser executado a partir do diretorio 'Code'.
REM
REM Uso:
REM   build.bat [comando]
REM
REM Comandos:
REM   build        - (Padrao) Configura e compila o projeto no diretorio 'build'.
REM   rebuild      - Limpa o diretorio de build e compila o projeto do zero.
REM   clean        - Apaga o diretorio de build.
REM   help, /?     - Mostra esta mensagem de ajuda.
REM
REM ============================================================================

REM --- Configuracao ---
SET BUILD_DIR=build
REM Adapte o gerador para a sua versao do Visual Studio, se necessario.
REM Exemplos: "Visual Studio 17 2022", "Visual Studio 16 2019"
SET CMAKE_GENERATOR="Visual Studio 17 2022"

REM --- Logica do Script ---
SET COMMAND=%1
IF /I "%COMMAND%"=="" SET COMMAND=build

ECHO Acao: %COMMAND%
ECHO.

IF /I "%COMMAND%"=="help" GOTO show_help
IF /I "%COMMAND%"=="/?" GOTO show_help
IF /I "%COMMAND%"=="build" GOTO do_build
IF /I "%COMMAND%"=="rebuild" GOTO do_rebuild
IF /I "%COMMAND%"=="clean" GOTO do_clean

ECHO Comando invalido: '%COMMAND%'. Use 'build.bat help' para ver as opcoes.
GOTO end

:do_build
ECHO --- Configurando o projeto com CMake (se necessario)... ---
IF NOT EXIST %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

REM Gera os arquivos de projeto se nao existirem
cmake -S . -B %BUILD_DIR% -G %CMAKE_GENERATOR%
IF %ERRORLEVEL% NEQ 0 (
    ECHO Erro ao configurar o projeto com CMake.
    GOTO end
)

ECHO.
ECHO --- Compilando o projeto (Debug)... ---
cmake --build %BUILD_DIR% --config Debug
IF %ERRORLEVEL% NEQ 0 (
    ECHO Erro ao compilar o projeto.
    GOTO end
)

ECHO.
ECHO Build concluido com sucesso!
GOTO end

:do_rebuild
CALL :do_clean
CALL :do_build
GOTO end

:do_clean
ECHO --- Limpando o diretorio de build... ---
IF EXIST %BUILD_DIR% (
    rmdir /S /Q %BUILD_DIR%
    ECHO Diretorio '%BUILD_DIR%' removido.
) ELSE (
    ECHO Diretorio '%BUILD_DIR%' nao encontrado. Nada a fazer.
)
ECHO.
GOTO end

:show_help
ECHO.
ECHO Uso: build.bat [comando]
ECHO.
ECHO Comandos:
ECHO   build        - (Padrao) Configura e compila o projeto no diretorio 'build'.
ECHO   rebuild      - Limpa o diretorio de build e compila o projeto do zero.
ECHO   clean        - Apaga o diretorio de build.
ECHO   help, /?     - Mostra esta mensagem de ajuda.
ECHO.
GOTO end

:end
endlocal
