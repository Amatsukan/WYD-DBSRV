#pragma once
#include "../common.h"
class Server; // Forward declaration

struct MSG_AccountLogin;
class MessageHandler {
    public:
    MessageHandler(Server& server);
    void onUserConnect(int sessionId);
    void onUserDisconnect(int sessionId);
    void onAdminConnect(int sessionId);
    void onAdminDisconnect(int sessionId);
    
    void processUserData(int sessionId);
    void processAdminData(int sessionId);
    
    
    private:
    void processClientMessage(int sessionId, char* msg);
    void processAdminMessage(int sessionId, char* msg);
    void sendSignalToAdmin(int sessionId, unsigned short signal, int parm = 0);
    void sendMessageToAdmin(int sessionId, const std::string& message);
    void sendStateToAdmin(int sessionId, unsigned short signal, const char* pass);
    
    void enableAccount(int adminSessionId, const std::string& accountName);
    void disableAccount(int adminSessionId, const std::string& accountName, int year, int yearDay);
    void convertGuildNumber(int group, STRUCT_ITEM* item);

    void handleAccountLogin(int sessionId, MSG_AccountLogin* msg);
    
    Server& m_server;
};