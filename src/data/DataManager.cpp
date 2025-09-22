#include "DataManager.h"
#include "../log/logger.h"
#include "../Server.h"
#include "../db/DatabaseManager.h"
#include "RankingSystem.h"
DataManager::DataManager(Server& server)
: m_server(server)
{
    m_rankingSystem = std::make_unique<RankingSystem>(m_gameData, m_server.getDatabase());
}
DataManager::~DataManager() = default;
void DataManager::initialize() {
    m_gameData.loadBaseMobs();
    loadGuildInfo();
    loadRanking();
    time_t now;
    time(&now);
    struct tm when;
    localtime_s(&when, &now);
    m_lastLogDay = when.tm_mday;
    m_lastDayLogDay = when.tm_mday;
}
#include "DataManager_Methods.inl"