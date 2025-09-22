#include "MessageHandler.h"
#include "../Server.h"
#include "../data/DataManager.h"
#include "../db/DatabaseManager.h"
#include "../log/logger.h"
#include "../session/SessionManager.h"

// ... (todo o código do MessageHandler.cpp, incluindo os handlers para cada tipo de mensagem)
// Devido ao tamanho extremo, vou mostrar um handler principal e a estrutura.

void MessageHandler::processClientMessage(int sessionId, char* msg) {
    MSG_STANDARD* header = reinterpret_cast<MSG_STANDARD*>(msg);

    if (!(header->Type & FLAG_GAME2DB)) {
        Logger::Log("Pacote invalido recebido do GameServer " + std::to_string(sessionId), "MessageHandler", true);
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
    
    // ... outras validações ...
    
    auto session = userSessions.getSession(sessionId);
    if(session) {
        session->accountFile = accFile; // Supondo que você adicione 'accountFile' a UserSession
        session->state = SessionState::Authenticated;
        
        MSG_DBCNFAccountLogin sm{};
        sm.Type = _MSG_DBCNFAccountLogin;
        sm.Size = sizeof(MSG_DBCNFAccountLogin);
        sm.ID = msg->ID;
        strncpy_s(sm.AccountName, accFile.Info.AccountName, _TRUNCATE);
        memcpy(sm.Cargo, accFile.Cargo, sizeof(sm.Cargo));
        sm.Coin = accFile.Coin;
        BASE_GetSelChar(&sm.sel, &accFile);
        
        m_server.sendToUser(sessionId, (char*)&sm, sm.Size);
    }
}