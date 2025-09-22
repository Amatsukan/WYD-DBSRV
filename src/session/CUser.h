#pragma once
#include "../common.h"

class CUser
{
    public:
    unsigned int IP;
    int Mode;
    int Count;
    int Level;
    int Encode1;
    int Encode2;
    char Name[ACCOUNTNAME_LENGTH];
    int DisableID;
    int Year;
    int YearDay;
    CUser() {
        memset(this, 0, sizeof(CUser));
        Level = -1;
    }
    
    
};