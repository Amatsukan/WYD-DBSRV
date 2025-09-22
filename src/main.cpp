#include "Server.h"
#include "log/Logger.h"

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
