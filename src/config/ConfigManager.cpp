#include "ConfigManager.h"
#include "../log/logger.h"
#include <Windows.h> // For MessageBoxA

void ConfigManager::createExampleConfig(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "# Arquivo de configuracao principal do DBSrv\n";
        file << "# Sapphire: valor de exemplo (padrao: 8)\n";
        file << "Sapphire " << m_sapphire << "\n\n";
        file << "# LastCapsule: valor de exemplo (padrao: 0)\n";
        file << "LastCapsule " << m_lastCapsule << "\n";
        Logger::Log("Criado arquivo de configuracao padrao: " + filename, "Config");
    }
}

bool ConfigManager::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        Logger::Log("AVISO: Nao foi possivel encontrar " + filename + ". Criando um novo com valores padrão.", "Config");
        createExampleConfig(filename);
        
        file.open(filename);
        if (!file.is_open()) {
            Logger::Log("ERRO: Falha ao criar e abrir o arquivo de configuração " + filename, "Config");
            MessageBoxA(NULL, ("Falha ao criar e abrir o arquivo de configuração: " + filename).c_str(), "Erro Crítico", MB_OK | MB_ICONERROR);
            return false;
        }
        MessageBoxA(NULL, ("Arquivo '" + filename + "' não encontrado. Um novo foi criado com valores padrão.").c_str(), "Aviso de Configuração", MB_OK | MB_ICONWARNING);
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
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
    file << "# Arquivo de configuração principal do DBSrv\n";
    file << "# Sapphire: valor de exemplo (padrão: 8)\n";
    file << "Sapphire " << m_sapphire << "\n\n";
    file << "# LastCapsule: valor de exemplo (padrão: 0)\n";
    file << "LastCapsule " << m_lastCapsule << "\n";
    
    Logger::Log("Configuracoes salvas em " + filename, "Config");
}