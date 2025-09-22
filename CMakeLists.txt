# Define a versão mínima do CMake e o nome do projeto.
cmake_minimum_required(VERSION 3.10)
project(DBSrv CXX)

# Adiciona o executável
add_executable(DBSrv
    # Ponto de entrada e classes principais
    src/main.cpp
    src/Server.cpp

    # Módulos
    src/config/ConfigManager.cpp
    src/log/logger.cpp
    src/data/GameData.cpp
    src/data/DataManager.cpp
    src/data/RankingSystem.cpp
    src/db/DatabaseManager.cpp
    src/message/MessageHandler.cpp
    src/network/NetworkManager.cpp
    src/session/SessionManager.cpp
    src/task/TaskManager.cpp

    # Bibliotecas comuns
    common.lib/Basedef.cpp
    common.lib/CPSock.cpp
)

# Adiciona os diretórios que contêm arquivos de cabeçalho (.h).
target_include_directories(DBSrv PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/common.lib
)

# Linka as bibliotecas necessárias do Windows
target_link_libraries(DBSrv PRIVATE
    ws2_32      # Para funções de rede (Winsock)
    winmm       # Para timeGetTime
)
