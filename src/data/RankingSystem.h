#pragma once
#include "../common.h"
class GameData;
class DatabaseManager;
// ... (definições de GrindRanking, etc. do CRanking.h original)
class RankingSystem
{
    public:
    RankingSystem(GameData& gameData, DatabaseManager& dbManager);
    void loadRanking();
    void writeRanking();
    private:
    int readAccountsInDir(const std::string& dir);
    // ... outros métodos privados ...
    GameData& m_gameData;
    DatabaseManager& m_dbManager;
    // ... membros do ranking ...
    
    
};