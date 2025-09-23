#include "Server.h"
#include "log/Logger.h"
#include <Windows.h>

// Dummy implementations for missing symbols from common.lib
HWND hWndMain = NULL;
unsigned int CurrentTime = 0;
unsigned int LastSendTime = 0;

void Log(char *String1, char *String2, unsigned int ip) {
    // Redirect to the new logger
    Logger::Log(std::string(String1) + " " + std::string(String2), "Legacy");
}

int main() {
    Server server;

    if (server.init()) {
        server.run();
    } else {
        // O Logger pode não ter sido inicializado, então usamos cout como fallback.
        std::cout << "Falha ao inicializar o servidor. Desligando." << std::endl;
        Logger::Log("Falha ao inicializar o servidor. Desligando.", "FATAL");
        // Pausa para que o usuário possa ler o erro no console antes de fechar.
        std::cout << "Pressione Enter para sair..." << std::endl;
        std::cin.get();
    }

    return 0;
}