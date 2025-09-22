#pragma once
#include "../common.h"
class GameData {
    public:
    STRUCT_GUILDINFO GuildInfo[65536];
    unsigned short g_pGuildWar[65536];
    unsigned short g_pGuildAlly[65536];
    STRUCT_ITEMLOG ItemDayLog[MAX_ITEMLIST];
    STRUCT_MOB g_pBaseSet[MAX_CLASS];
    int ServerIndex = -1;
    int Sapphire = 8;
    unsigned short LastCapsule = 0;
    char g_pServerList[MAX_SERVERGROUP][MAX_SERVERNUMBER][64];
    
    GameData();
    void loadBaseMobs();
    
    
};