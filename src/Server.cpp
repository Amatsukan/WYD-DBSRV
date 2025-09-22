#include "Server.h"
#include "config/ConfigManager.h"
#include "log/logger.h"
#include "network/NetworkManager.h"
#include "session/SessionManager.h"
#include "message/MessageHandler.h"
#include "task/TaskManager.h"
#include "data/DataManager.h"
#include "db/DatabaseManager.h"
// Construtor inicializa todos os módulos na ordem correta usando ponteiros inteligentes
Server::Server() {
    m_config = std::make_unique<ConfigManager>();
    m_database = std::make_unique<DatabaseManager>();
    m_userSessions = std::make_unique<SessionManager>(MAX_SERVER, "User");
    m_adminSessions = std::make_unique<SessionManager>(MAX_ADMIN, "Admin");
    m_network = std::make_unique<NetworkManager>(*m_userSessions, *m_adminSessions);
    m_dataManager = std::make_unique<DataManager>(*this);
    m_messageHandler = std::make_unique<MessageHandler>(*this);
    m_taskManager = std::make_unique<TaskManager>(*this);
    m_adminIPs.resize(MAX_ADMIN, 0);
    
    
}
Server::~Server() {
    shutdown();
}
bool Server::init() {
    Logger::Init("DBServidor.log");
    Logger::Log("Iniciando servidor...", "Server");
    readAdminIPs();
    
    if (!m_config->load("Config.txt")) {
        return false;
    }
    
    if (!initializeServerIndex()) {
        return false;
    }
    
    auto& gameData = m_dataManager->getGameData();
    gameData.Sapphire = m_config->getSapphire();
    gameData.LastCapsule = m_config->getLastCapsule();
    
    m_dataManager->initialize();
    
    m_network->setUserCallbacks(
        [this](int id) { this->onUserConnect(id); },
        [this](int id) { this->onUserDisconnect(id); },
        [this](int id) { this->onUserData(id); }
    );
    m_network->setAdminCallbacks(
        [this](int id) { this->onAdminConnect(id); },
        [this](int id) { this->onAdminDisconnect(id); }, 
        [this](int id) { this->onAdminData(id); }
    );
    
    if (!m_network->start(DB_PORT, ADMIN_PORT)) {
        return false;
    }
    
    m_taskManager->start();
    return true;
    
    
}
#include "Server_Methods.inl"