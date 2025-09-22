#pragma once
#include "common.h"
// Forward declarations para evitar includes circulares
class ConfigManager;
class Logger;
class NetworkManager;
class SessionManager;
class MessageHandler;
class TaskManager;
class DataManager;
class DatabaseManager;
class Server {
    public:
    Server();
    ~Server();
    bool init();
    void run();
    void shutdown();
    
    void processTick();
    
    // Funções de comunicação para serem usadas por outros módulos
    void sendToUser(int serverId, const char* buffer, size_t size);
    void broadcastToUsers(const char* buffer, size_t size);
    void sendToAdmin(int sessionId, const char* buffer, size_t size);
    void disconnectSession(int sessionId, bool isAdmin);
    
    // Getters para acesso dos módulos
    DataManager& getDataManager() { return *m_dataManager; }
    DatabaseManager& getDatabase() { return *m_database; }
    SessionManager& getUserSessions() { return *m_userSessions; }
    SessionManager& getAdminSessions() { return *m_adminSessions; }
    
    
    private:
    void readAdminIPs();
    bool initializeServerIndex();
    // Callbacks para o NetworkManager
    void onUserConnect(int sessionId);
    void onUserDisconnect(int sessionId);
    void onUserData(int sessionId);
    
    void onAdminConnect(int sessionId);
    void onAdminDisconnect(int sessionId);
    void onAdminData(int sessionId);
    
    void handleConsoleInput();
    
    std::atomic<bool> m_isRunning{false};
    
    // Módulos principais
    std::unique_ptr<ConfigManager> m_config;
    std::unique_ptr<DatabaseManager> m_database;
    std::unique_ptr<SessionManager> m_userSessions;
    std::unique_ptr<SessionManager> m_adminSessions;
    std::unique_ptr<NetworkManager> m_network;
    std::unique_ptr<DataManager> m_dataManager;
    std::unique_ptr<MessageHandler> m_messageHandler;
    std::unique_ptr<TaskManager> m_taskManager;
    
    std::thread m_consoleThread;
    std::vector<unsigned int> m_adminIPs;
    
    
};