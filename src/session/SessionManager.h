#pragma once
#include "../common.h"
#include "UserSession.h"

// Classe para gerenciar sessões de cliente de forma thread-safe
class SessionManager {
public:
    SessionManager(size_t maxSessions);
    int createSession(SOCKET socket, const std::string& ip);
    std::shared_ptr<UserSession> getSession(int sessionId);
    void removeSession(int sessionId);
    void printStatus();

private:
    std::vector<std::shared_ptr<UserSession>> m_sessions;
    std::mutex m_mutex;
    size_t m_maxSessions;
};
