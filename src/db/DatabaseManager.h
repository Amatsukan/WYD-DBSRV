#pragma once
#include "../common.h"
class DatabaseManager {
    public:
    DatabaseManager() = default;
    std::optional<STRUCT_ACCOUNTFILE> readAccount(const std::string& accountName);
    bool writeAccount(const STRUCT_ACCOUNTFILE& accountFile);
    bool accountExists(const std::string& accountName);
    
    bool createCharacter(const std::string& accountName, const std::string& charName);
    bool deleteCharacter(const std::string& charName);
    std::string getAccountByChar(const std::string& charName);
    
    std::vector<std::string> getAllAccountFiles();
    
    
    private:
    std::string getAccountPath(const std::string& accountName);
    std::string getCharPath(const std::string& charName);
    std::string getFirstKey(const std::string& source);
};