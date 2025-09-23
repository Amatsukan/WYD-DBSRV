# 6. Processo de Build (Windows)

A compilação do projeto `DBSrv` em ambiente Windows é gerenciada pelo script `build.bat`. Este script automatiza o uso do CMake e do compilador para gerar e construir o executável.

## Localização

O script está localizado na raiz do projeto: `c:\Users\amatsukan\wyd\WYD-Server-CPP\Code\DBSrv\build.bat`.

## Comandos Disponíveis

Para utilizar o script, abra um terminal no diretório do projeto e execute `build.bat` seguido por um dos comandos abaixo.

### `build` (Padrão)

Este é o comando padrão, executado se nenhum outro for especificado. Ele realiza duas ações principais:
1.  **Configuração:** Executa o CMake (`cmake -S . -B build`) para gerar os arquivos de projeto do Visual Studio no diretório `build/`. Esta etapa só é executada se o diretório `build/` não existir ou precisar ser atualizado.
2.  **Compilação:** Compila o projeto usando o CMake (`cmake --build build`), que por sua vez invoca o compilador (MSVC) para gerar o executável `DBSrv.exe`.

**Uso:**
```shell
build.bat build
```
ou simplesmente
```shell
build.bat
```

### `compile`

Este comando executa **apenas** a etapa de compilação, pulando a geração de arquivos de projeto pelo CMake. É útil para compilar rapidamente após fazer alterações no código-fonte, sem reconfigurar todo o projeto.

**Uso:**
```shell
build.bat compile
```

### `rebuild`

Este comando limpa completamente o ambiente de build e o reconstrói do zero. Ele executa as seguintes ações:
1.  Apaga o diretório `build/`.
2.  Executa o comando `build` para reconfigurar e recompilar o projeto.

**Uso:**
```shell
build.bat rebuild
```

### `clean`

Remove o diretório `build/` e os artefatos de compilação (`DBSrv.exe`, `DBSrv.pdb`), limpando todos os arquivos gerados pelo processo de build.

**Uso:**
```shell
build.bat clean
```

### `help` ou `/?`

Mostra a mensagem de ajuda diretamente no terminal, listando todos os comandos disponíveis e uma breve descrição de cada um.

**Uso:**
```shell
build.bat help
```
