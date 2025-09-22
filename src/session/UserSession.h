#pragma once
#include "../common.h"

// Representa o estado de uma sessão
enum class SessionState {
    Connecting,
    Connected,
    Authenticated,
    Disconnected
};

// Estrutura para manter os dados de uma sessão de cliente (usuário ou admin)
struct UserSession {
    SOCKET socket = INVALID_SOCKET;
    std::string ipAddress;
    SessionState state = SessionState::Disconnected;
    CUser userData; // Mantém a estrutura CUser original para compatibilidade
    CPSock sockHelper; // Usa a classe CPSock para buffer e leitura de mensagens

    UserSession() = default;

    // Construtor de movimentação para eficiência
    UserSession(UserSession&& other) noexcept {
        socket = other.socket;
        ipAddress = std::move(other.ipAddress);
        state = other.state;
        userData = other.userData;
        sockHelper = std::move(other.sockHelper);
        other.socket = INVALID_SOCKET;
    }
    UserSession& operator=(UserSession&& other) noexcept {
        if (this != &other) {
            socket = other.socket;
            ipAddress = std::move(other.ipAddress);
            state = other.state;
            userData = other.userData;
            sockHelper = std::move(other.sockHelper);
            other.socket = INVALID_SOCKET;
        }
        return *this;
    }

    // Desabilita cópia para evitar problemas de posse do socket
    UserSession(const UserSession&) = delete;
    UserSession& operator=(const UserSession&) = delete;
};
