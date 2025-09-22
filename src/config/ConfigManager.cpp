#include "ConfigManager.h"
#include "../log/logger.h"
bool ConfigManager::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        Logger::Log("ERRO: Nao foi possivel encontrar " + filename, "Config");
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key;
        ss >> key;
        if (key == "Sapphire") {
            ss >> m_sapphire;
        } else if (key == "LastCapsule") {
            ss >> m_lastCapsule;
        }
    }
    
    Logger::Log("Configuracoes carregadas de " + filename, "Config");
    return true;
    
    
}
void ConfigManager::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        Logger::Log("ERRO: Nao foi possivel salvar em " + filename, "Config");
        return;
    }
    file << "Sapphire " << m_sapphire << "\n";
    file << "LastCapsule " << m_lastCapsule << "\n";
    
    Logger::Log("Configuracoes salvas em " + filename, "Config");
    
    
}