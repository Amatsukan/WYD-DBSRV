cmake_minimum_required(VERSION 3.10)
project(DBSrv CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontra automaticamente todos os arquivos .cpp dentro do diretório src.
# NOTA: Ao adicionar um novo arquivo .cpp, pode ser necessário rodar o CMake novamente.
file(GLOB_RECURSE SRV_SOURCES "src/*.cpp")

# Encontra os fontes da biblioteca comum.
file(GLOB COMMON_LIB_SOURCES "common.lib/*.cpp")

# Define os fontes do executável
set(DBSrv_SOURCES
    ${SRV_SOURCES}
    ${COMMON_LIB_SOURCES}
)

# Adiciona o executável
add_executable(DBSrv ${DBSrv_SOURCES})

# Set the output directory for the executable to the project root
set_target_properties(DBSrv PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Adiciona os diretórios de inclusão para os cabeçalhos.
target_include_directories(DBSrv PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/common.lib"
)

# Configurações e bibliotecas específicas para Windows
if(WIN32)
    # Adiciona o arquivo de recurso e o modo de aplicação Windows
    target_sources(DBSrv PRIVATE DBSrv.rc)
    

    # Adiciona a flag para tratar warnings como erros
    target_compile_options(DBSrv PRIVATE)

    # Adiciona a definição para desabilitar warnings de depreciação
    target_compile_definitions(DBSrv PRIVATE _CRT_SECURE_NO_WARNINGS)

    # Linka as bibliotecas do Windows necessárias.
    target_link_libraries(DBSrv PRIVATE
        ws2_32
        winmm
    )
endif()

# Configurações e bibliotecas específicas para Linux
if(UNIX AND NOT APPLE)
    # Adiciona a flag para tratar warnings como erros
    target_compile_options(DBSrv PRIVATE -Werror)

    # Bibliotecas equivalentes ou necessárias no Linux
    target_link_libraries(DBSrv PRIVATE
        pthread
    )
endif()