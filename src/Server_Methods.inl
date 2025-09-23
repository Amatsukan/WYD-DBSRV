#include "log/logger.h"
#include "Server.h"

// Este arquivo contém a implementação dos métodos da classe Server para manter Server.cpp mais limpo.
void Server::run() {
    m_isRunning = true;
    Logger::Log("Servidor em execucao. Digite 'exit' para desligar, 'status' para ver sessoes.", "Server");
    m_consoleThread = std::thread(&Server::handleConsoleInput, this);
    if (m_consoleThread.joinable()) {
        m_consoleThread.join();
    }
    
    
}
void Server::shutdown() {
    if (!m_isRunning.exchange(false)) {
        return;
    }
    Logger::Log("Iniciando desligamento...", "Server");
    
    m_taskManager->stop();
    m_network->stop();
    
    auto& gameData = m_dataManager->getGameData();
    m_config->setSapphire(gameData.Sapphire);
    m_config->setLastCapsule(gameData.LastCapsule);
    m_config->save("../config/Config.txt");
    
    m_dataManager->writeGuildInfo(); 
    Logger::Shutdown();
    
    
}
void Server::processTick() {
    m_dataManager->periodicUpdate();
}

void Server::processMinTimer() {
    m_dataManager->minuteUpdate();
}
void Server::sendToUser(int serverId, const char* buffer, size_t size) {
    auto session = m_userSessions->getSession(serverId);
    if (session && session->socket != INVALID_SOCKET) {
        session->sockHelper.SendOneMessage((char*)buffer, (int)size);
    }
}
void Server::broadcastToUsers(const char* buffer, size_t size) {
    for (int i = 0; i < MAX_SERVER; ++i) {
        sendToUser(i, buffer, size);
    }
}
void Server::sendToAdmin(int sessionId, const char* buffer, size_t size) {
    auto session = m_adminSessions->getSession(sessionId);
    if (session && session->socket != INVALID_SOCKET) {
        session->sockHelper.SendOneMessage((char*)buffer, (int)size);
    }
}
void Server::disconnectSession(int sessionId, bool isAdmin) {
    if (isAdmin) {
        m_adminSessions->removeSession(sessionId);
    } else {
        m_userSessions->removeSession(sessionId);
    }
}

void Server::createExampleAdminFile() {
    // Garante que o diretório de configuração exista
    std::filesystem::path configDir("../config");
    if (!std::filesystem::exists(configDir)) {
        std::filesystem::create_directory(configDir);
    }

    std::ofstream file("../config/Admin.txt");
    if (file.is_open()) {
        file << "# Este arquivo lista os IPs que tem permissão de administrador.\n";
        file << "# O formato é: [índice] [IP]\n";
        file << "# Exemplo:\n";
        file << "0 127.0.0.1\n";
    }
}

void Server::readAdminIPs() {
    std::ifstream file("../config/Admin.txt");
    if (!file.is_open()) {
        Logger::Log("AVISO: Arquivo Admin.txt nao encontrado. Criando um novo com exemplo.", "Server");
        createExampleAdminFile();
        
        file.open("../config/Admin.txt");
        if (!file.is_open()) {
            Logger::Log("ERRO: Falha ao criar e abrir o arquivo ../config/Admin.txt", "Server");
            MessageBoxA(NULL, "Falha ao criar e abrir o arquivo ../config/Admin.txt", "Erro Crítico", MB_OK | MB_ICONERROR);
            return;
        }
        MessageBoxA(NULL, "Arquivo '../config/Admin.txt' não encontrado.\nUm novo foi criado com o IP 127.0.0.1 como exemplo.", "Aviso de Configuração", MB_OK | MB_ICONWARNING);
    }
    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        int idx;
        unsigned int a, b, c, d;
        std::string ip_str = line;
        std::replace(line.begin(), line.end(), '.', ' ');
        std::stringstream ss(line);
        if (ss >> idx >> a >> b >> c >> d) {
            if (idx >= 0 && idx < MAX_ADMIN) {
                m_adminIPs[idx] = (d << 24) | (c << 16) | (b << 8) | a;
                Logger::Log("Admin IP carregado - Indice: " + std::to_string(idx) + ", IP: " + ip_str, "Server");
                count++;
            }
        }
    }
    Logger::Log(std::to_string(count) + " IPs de admin carregados.", "Server");
}

bool Server::initializeServerIndex() {
    auto& gameData = m_dataManager->getGameData();
    BASE_InitializeServerList();
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        Logger::Log("Falha ao obter o hostname.", "Server", true);
        return false;
    }
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    if (getaddrinfo(hostname, nullptr, &hints, &res) != 0) {
        Logger::Log("Falha ao obter o addrinfo.", "Server", true);
        return false;
    }
    int serverIndex = -1;
    Logger::Log("Procurando por IPs locais no serverlist.txt...", "Server");
    for (auto p = res; p != nullptr; p = p->ai_next) {
        if (p->ai_family != AF_INET) continue;
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(p->ai_family, &((sockaddr_in*)p->ai_addr)->sin_addr, ipStr, sizeof(ipStr));
        Logger::Log(std::string("IP local encontrado: ") + ipStr, "Server");
        for (int i = 0; i < MAX_SERVERGROUP; i++) {
            if (strcmp(g_pServerList[i][0], ipStr) == 0) {
                serverIndex = i;
                break;
            }
        }
        if (serverIndex != -1) break;
    }
    freeaddrinfo(res);
    if (serverIndex == -1) {
        Logger::Log("IP local nao encontrado no serverlist.txt. Verifique a configuracao.", "Server", true);
        return false;
    }
    gameData.ServerIndex = serverIndex;
    Logger::Log("Indice do Servidor definido como: " + std::to_string(serverIndex), "Server");
    return true;
    
    
}
void Server::onUserConnect(int sessionId) {
    Logger::Log("GameServer conectado no slot " + std::to_string(sessionId), "Server");
    m_messageHandler->onUserConnect(sessionId);
}
void Server::onUserDisconnect(int sessionId) {
    Logger::Log("GameServer desconectado do slot " + std::to_string(sessionId), "Server");
    m_messageHandler->onUserDisconnect(sessionId);
}
void Server::onUserData(int sessionId) {
    m_messageHandler->processUserData(sessionId);
}
void Server::onAdminConnect(int sessionId) {
    auto session = m_adminSessions->getSession(sessionId);
    if (!session) return;
    unsigned int clientIp = 0;
    inet_pton(AF_INET, session->ipAddress.c_str(), &clientIp);
    
    bool isAllowed = false;
    for (unsigned int adminIp : m_adminIPs) {
        if (adminIp != 0 && adminIp == clientIp) {
            isAllowed = true;
            break;
        }
    }
    if (!isAllowed) {
        Logger::Log("Conexao de Admin REJEITADA de IP nao autorizado: " + session->ipAddress, "Server");
        disconnectSession(sessionId, true);
        return;
    }
    Logger::Log("Admin conectado do IP: " + session->ipAddress, "Server");
    m_messageHandler->onAdminConnect(sessionId);
    
    
}
void Server::onAdminDisconnect(int sessionId) {
    Logger::Log("Admin desconectado do slot " + std::to_string(sessionId), "Server");
    m_messageHandler->onAdminDisconnect(sessionId);
}
void Server::onAdminData(int sessionId) {
    m_messageHandler->processAdminData(sessionId);
}
void Server::handleConsoleInput() {
    std::string line;
    while (m_isRunning) {
        std::getline(std::cin, line);
        if (line == "exit") {
            shutdown();
        } else if (line == "status") {
            Logger::Log("--- Status das Sessoes ---", "Console");
            m_userSessions->printStatus();
            m_adminSessions->printStatus();
            Logger::Log("--------------------------", "Console");
        } else if (line == "drawconfig") {
            drawConfig();
        }
    }
}