#include "GameData.h"
#include "../log/logger.h"
#include <fcntl.h>
#include <io.h>
GameData::GameData() {
    memset(GuildInfo, 0, sizeof(GuildInfo));
    memset(g_pGuildWar, 0, sizeof(g_pGuildWar));
    memset(g_pGuildAlly, 0, sizeof(g_pGuildAlly));
    memset(ItemDayLog, 0, sizeof(ItemDayLog));
    memset(g_pBaseSet, 0, sizeof(g_pBaseSet));
    memset(g_pServerList, 0, sizeof(g_pServerList));
}
void GameData::loadBaseMobs() {
    Logger::Log("Carregando status base dos personagens...", "GameData");
    int handle = _open("./BaseMob/TK", _O_RDONLY | _O_BINARY);
    if(handle == -1){ Logger::Log("ERRO: Nao foi possivel carregar ./BaseMob/TK", "GameData", true); return; }
    _read(handle, &g_pBaseSet[0], sizeof(STRUCT_MOB)); _close(handle);
    
    handle = _open("./BaseMob/FM", _O_RDONLY | _O_BINARY);
    if(handle == -1){ Logger::Log("ERRO: Nao foi possivel carregar ./BaseMob/FM", "GameData", true); return; }
    _read(handle, &g_pBaseSet[1], sizeof(STRUCT_MOB)); _close(handle);
    
    handle = _open("./BaseMob/BM", _O_RDONLY | _O_BINARY);
    if(handle == -1){ Logger::Log("ERRO: Nao foi possivel carregar ./BaseMob/BM", "GameData", true); return; }
    _read(handle, &g_pBaseSet[2], sizeof(STRUCT_MOB)); _close(handle);
    
    handle = _open("./BaseMob/HT", _O_RDONLY | _O_BINARY);
    if(handle == -1){ Logger::Log("ERRO: Nao foi possivel carregar ./BaseMob/HT", "GameData", true); return; }
    _read(handle, &g_pBaseSet[3], sizeof(STRUCT_MOB)); _close(handle);
    
    g_pBaseSet[0].BaseScore = g_pBaseSet[0].CurrentScore;
    g_pBaseSet[1].BaseScore = g_pBaseSet[1].CurrentScore;
    g_pBaseSet[2].BaseScore = g_pBaseSet[2].CurrentScore;
    g_pBaseSet[3].BaseScore = g_pBaseSet[3].CurrentScore;
    Logger::Log("Status base carregados com sucesso.", "GameData");
    
    
}