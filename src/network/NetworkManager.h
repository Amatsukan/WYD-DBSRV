#pragma once
#include "../common.h"

// Callbacks para o Server orquestrar a lógica
using OnConnectCallback = std::function<void(int sessionId)>;
using OnDisconnectCallback = std::function<void(int sessionId)>;
using OnDataReceivedCallback = std::function<void(int sessionId)>;

class SessionManager; // Forward declaration

class NetworkManager {
public:
    NetworkManager(SessionManager& userSessions, SessionManager& adminSessions);
    ~NetworkManager();

    bool start(int userPort, int adminPort);
    void stop();

    void setUserCallbacks(OnConnectCallback onConnect, OnDisconnectCallback onDisconnect, OnDataReceivedCallback onData);
    void setAdminCallbacks(OnConnectCallback onConnect, OnDisconnectCallback onDisconnect, OnDataReceivedCallback onData);


private:
    void acceptLoop(SOCKET listenSocket, SessionManager& sessionManager);
    void clientHandlerLoop(int sessionId, SessionManager& sessionManager);

    SOCKET createUserListenSocket(int port);
    
    std::atomic<bool> m_isRunning{false};
    SOCKET m_userListenSocket = INVALID_SOCKET;
    SOCKET m_adminListenSocket = INVALID_SOCKET;
    
    std::vector<std::thread> m_threads;
    
    SessionManager& m_userSessions;
    SessionManager& m_adminSessions;

    OnConnectCallback m_onUserConnect;
    OnConnectCallback m_onAdminConnect;
    OnDisconnectCallback m_onDisconnect;
    OnDataReceivedCallback m_onUserData;
    OnDataReceivedCallback m_onAdminData
};
