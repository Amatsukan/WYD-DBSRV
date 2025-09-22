#include "NetworkManager.h"
#include "../session/SessionManager.h"
#include "../log/Logger.h"

NetworkManager::NetworkManager(SessionManager& userSessions, SessionManager& adminSessions)
    : m_userSessions(userSessions), m_adminSessions(adminSessions) {}

NetworkManager::~NetworkManager() {
    stop();
}

bool NetworkManager::start(int userPort, int adminPort) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Logger::Log("WSAStartup falhou.", "Network");
        return false;
    }

    m_userListenSocket = createUserListenSocket(userPort);
    m_adminListenSocket = createUserListenSocket(adminPort);

    if (m_userListenSocket == INVALID_SOCKET || m_adminListenSocket == INVALID_SOCKET) {
        return false;
    }

    m_isRunning = true;
    m_threads.emplace_back(&NetworkManager::acceptLoop, this, m_userListenSocket, std::ref(m_userSessions));
    m_threads.emplace_back(&NetworkManager::acceptLoop, this, m_adminListenSocket, std::ref(m_adminSessions));

    Logger::Log("NetworkManager iniciado.", "Network");
    return true;
}

void NetworkManager::stop() {
    m_isRunning = false;

    if (m_userListenSocket != INVALID_SOCKET) {
        closesocket(m_userListenSocket);
        m_userListenSocket = INVALID_SOCKET;
    }
    if (m_adminListenSocket != INVALID_SOCKET) {
        closesocket(m_adminListenSocket);
        m_adminListenSocket = INVALID_SOCKET;
    }

    for (auto& t : m_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    WSACleanup();
    Logger::Log("NetworkManager parado.", "Network");
}

void NetworkManager::setCallbacks(OnConnectCallback onConnect, OnDisconnectCallback onDisconnect, OnDataReceivedCallback onData) {
    m_onConnect = onConnect;
    m_onDisconnect = onDisconnect;
    m_onDataReceived = onData;
}

SOCKET NetworkManager::createUserListenSocket(int port) {
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        Logger::Log("socket() falhou com erro: " + std::to_string(WSAGetLastError()), "Network");
        return INVALID_SOCKET;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);

    if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        Logger::Log("bind() falhou na porta " + std::to_string(port), "Network");
        closesocket(listenSocket);
        return INVALID_SOCKET;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        Logger::Log("listen() falhou.", "Network");
        closesocket(listenSocket);
        return INVALID_SOCKET;
    }

    Logger::Log("Escutando na porta " + std::to_string(port), "Network");
    return listenSocket;
}

void NetworkManager::acceptLoop(SOCKET listenSocket, SessionManager& sessionManager) {
    while (m_isRunning) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            if (m_isRunning) { // Evita mensagem de erro ao desligar
                Logger::Log("accept() falhou.", "Network");
            }
            continue;
        }

        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);

        int sessionId = sessionManager.createSession(clientSocket, clientIp);
        if (sessionId != -1) {
            if (m_onConnect) m_onConnect(sessionId);
            m_threads.emplace_back(&NetworkManager::clientHandlerLoop, this, sessionId, std::ref(sessionManager));
        } else {
            closesocket(clientSocket);
        }
    }
}

void NetworkManager::clientHandlerLoop(int sessionId, SessionManager& sessionManager) {
    auto session = sessionManager.getSession(sessionId);
    if (!session) return;
    
    while (m_isRunning && session->state != SessionState::Disconnected) {
        // Usa o helper CPSock para receber dados de forma fragmentada
        if (session->sockHelper.Receive() != 1) {
            // Erro ou desconexão
            break;
        }

        // Se o recebimento foi bem-sucedido, invoca o callback de dados
        if (m_onDataReceived) {
            m_onDataReceived(sessionId);
        }
    }

    if (m_onDisconnect) m_onDisconnect(sessionId);
    sessionManager.removeSession(sessionId);
}
