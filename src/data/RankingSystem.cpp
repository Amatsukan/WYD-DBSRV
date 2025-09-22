#include "RankingSystem.h"
#include "GameData.h"
#include "../db/DatabaseManager.h"
#include "../log/logger.h"
RankingSystem::RankingSystem(GameData& gameData, DatabaseManager& dbManager)
: m_gameData(gameData), m_dbManager(dbManager) {}
void RankingSystem::loadRanking() {
    unsigned int startClock = GetTickCount();
    int entryCount = readAccountsInDir("./account");
    unsigned int resultClock = GetTickCount() - startClock;
    std::string logMsg = "Lidas " + std::to_string(entryCount) + " entradas de ranking em " 
    + std::to_string(resultClock / 1000) + "." + std::to_string(resultClock % 1000) + " segundos.";
    Logger::Log(logMsg, "Ranking");
    
    
}
int RankingSystem::readAccountsInDir(const std::string& dir) {
    // ... implementação usando std::filesystem e dbManager ...
    return 0;
}
void RankingSystem::writeRanking() {
    // ... implementação ...
}