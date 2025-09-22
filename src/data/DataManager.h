#pragma once
#include "../common.h"
#include "GameData.h"
// Forward declarations
class Server;
class RankingSystem;
class DataManager
{
    public:
    DataManager(Server& server);
    ~DataManager();
    void initialize();
    void periodicUpdate();
    void writeGuildInfo();
    
    GameData& getGameData() { return m_gameData; }
    
    
    private:
    void importItems();
    void importDonates();
    void importUsers();
    void updateUserPasswords();
    void loadGuildInfo();
    void resetWeeklyFame();
    
    void loadRanking();
    void writeRanking();
    
    void generateDailyLogs();
    void generateItemLog();
    void generateExpLog();
    void findAccountsForExpLog(const std::string& directory, std::ofstream& logFile);
    
    void updateConnectionStatus();
    void updateConnectionData();
    
    Server& m_server;
    GameData m_gameData;
    std::unique_ptr<RankingSystem> m_rankingSystem;
    
    int m_secCounter = 0;
    int m_lastLogDay = -1;
    int m_lastDayLogDay = -1;
    int m_userConnection[MAX_SERVER] = {0,};
    
    
};