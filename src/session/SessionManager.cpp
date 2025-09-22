#include "SessionManager.h"
#include "../log/Logger.h"

SessionManager::SessionManager(size_t maxSessions) : m_maxSessions(maxSessions) {
    m_sessions.resize(maxSessions, nullptr);
}

int SessionManager::createSession(SOCKET socket, const std::string& ip) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (size_t i = 0; i < m_maxSessions; ++i) {
        if (!m_sessions[i]) {
            m_sessions[i] = std::make_shared<UserSession>();
            m_sessions[i]->socket = socket;
            m_sessions[i]->ipAddress = ip;
            m_sessions[i]->state = SessionState::Connecting;
            m_sessions[i]->sockHelper.Sock = socket; // Configura o helper de socket
            Logger::Log("Sessao criada no slot " + std::to_string(i) + " para IP " + ip, "Session");
            return static_cast<int>(i);
        }
    }
    Logger::Log("Nao foi possivel criar sessao: Servidor cheio.", "Session");
    return -1;
}

std::shared_ptr<UserSession> SessionManager::getSession(int sessionId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (sessionId >= 0 && sessionId < m_maxSessions) {
        return m_sessions[sessionId];
    }
    return nullptr;
}

void SessionManager::removeSession(int sessionId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (sessionId >= 0 && sessionId < m_maxSessions && m_sessions[sessionId]) {
        closesocket(m_sessions[sessionId]->socket);
        m_sessions[sessionId] = nullptr;
        Logger::Log("Sessao removida do slot " + std::to_string(sessionId), "Session");
    }
}

void SessionManager::printStatus() {
    std::lock_guard<std::mutex> lock(m_mutex);
    int activeSessions = 0;
    for (const auto& session : m_sessions) {
        if (session) {
            activeSessions++;
            // TODO: Adicionar mais detalhes da sessão se necessário
        }
    }
    printf("Sessoes Ativas: %d/%zu\n", activeSessions, m_maxSessions);
}
