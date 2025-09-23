#include "MessageHandler.h"
#include "../Server.h"
#include "../data/DataManager.h"
#include "../db/DatabaseManager.h"
#include "../log/logger.h"
#include "../session/SessionManager.h"
#include "../../common.lib/Basedef.h"

// ... (todo o código do MessageHandler.cpp, incluindo os handlers para cada tipo de mensagem)
// Devido ao tamanho extremo, vou mostrar um handler principal e a estrutura.

void MessageHandler::processClientMessage(int sessionId, char* msg) {
    MSG_STANDARD* header = reinterpret_cast<MSG_STANDARD*>(msg);

    if (!(header->Type & FLAG_GAME2DB)) {
        Logger::Log("Pacote invalido recebido do GameServer " + std::to_string(sessionId), "MessageHandler");
        return;
    }

    switch (header->Type) {
        case _MSG_DBAccountLogin:
            handleAccountLogin(sessionId, reinterpret_cast<MSG_AccountLogin*>(msg));
            break;
        case _MSG_DBCreateCharacter:
            // ...
            break;
        // ... todos os outros cases
    }
}

void MessageHandler::handleAccountLogin(int sessionId, MSG_AccountLogin* msg) {
    auto& db = m_server.getDatabase();
    auto& userSessions = m_server.getUserSessions();

    char accountName[ACCOUNTNAME_LENGTH] = {0};
    strncpy_s(accountName, msg->AccountName, _TRUNCATE);
    
    char password[ACCOUNTPASS_LENGTH] = {0};
    strncpy_s(password, msg->AccountPassword, _TRUNCATE);

    auto accountOpt = db.readAccount(accountName);

    if (!accountOpt) {
        MSG_STANDARD sm{sizeof(MSG_STANDARD), 0,0, _MSG_DBAccountLoginFail_Account, (short)msg->ID, 0};
        m_server.sendToUser(sessionId, (char*)&sm, sm.Size);
        return;
    }

    STRUCT_ACCOUNTFILE accFile = *accountOpt;

    if (std::string(password) != std::string(accFile.Info.AccountPass)) {
        MSG_STANDARD sm{sizeof(MSG_STANDARD), 0,0, _MSG_DBAccountLoginFail_Pass, (short)msg->ID, 0};
        m_server.sendToUser(sessionId, (char*)&sm, sm.Size);
        return;
    }

}

MessageHandler::MessageHandler(Server& server) : m_server(server) {}
void MessageHandler::onUserConnect(int sessionId) {}
void MessageHandler::onUserDisconnect(int sessionId) {}
void MessageHandler::onAdminConnect(int sessionId) {}
void MessageHandler::onAdminDisconnect(int sessionId) {}

void MessageHandler::processUserData(int sessionId) {
    auto& sessions = m_server.getUserSessions();
    auto session = sessions.getSession(sessionId);
    if (session) {
        int errorCode = 0;
        int errorType = 0;
        char* msg = nullptr;
        while((msg = session->sockHelper.ReadMessage(&errorCode, &errorType)) != nullptr) {
            processClientMessage(sessionId, msg);
        }
    }
}

void MessageHandler::processAdminData(int sessionId) {
    auto& sessions = m_server.getAdminSessions();
    auto session = sessions.getSession(sessionId);
    if (session) {
        int errorCode = 0;
        int errorType = 0;
        char* msg = nullptr;
        while((msg = session->sockHelper.ReadMessage(&errorCode, &errorType)) != nullptr) {
            processAdminMessage(sessionId, msg);
        }
    }
}

void MessageHandler::processAdminMessage(int sessionId, char* msg) {}