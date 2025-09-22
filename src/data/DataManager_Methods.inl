// Implementação dos métodos do DataManager

void DataManager::periodicUpdate() {
    m_secCounter++;
    
    // A cada 30 segundos
    if (m_secCounter % 30 == 0) {
        importItems();
        importDonates();
        importUsers();
        updateUserPasswords();
        updateConnectionStatus();
    }
    // A cada minuto
    if (m_secCounter % 60 == 0) {
        updateConnectionData();
    }
    // A cada 10 minutos
    if (m_secCounter % 600 == 0) {
        writeRanking();
    }

    time_t now;
	time(&now);
	struct tm when;
    localtime_s(&when, &now);
    
    // Inicia um novo arquivo de log se o dia mudou
    if (when.tm_mday != m_lastLogDay) {
        Logger::Init("DBServidor.log"); 
        m_lastLogDay = when.tm_mday;
    }

    // Gera logs diários
    if(when.tm_mday != m_lastDayLogDay) {
        generateDailyLogs();
        m_lastDayLogDay = when.tm_mday;
    }

    // Reseta a fama da guilda semanalmente (domingo à meia-noite)
    if(when.tm_hour == 0 && when.tm_wday == 0 && when.tm_min == 0 && when.tm_sec == 0) {
        resetWeeklyFame();
    }
}

void DataManager::importItems() {
    const std::string importPath = "../../Common/ImportItem/";
    if (!std::filesystem::exists(importPath)) return;

    for (const auto& entry : std::filesystem::directory_iterator(importPath)) {
        if (!entry.is_regular_file()) continue;

        std::ifstream file(entry.path());
        if (!file.is_open()) continue;

        std::string line;
        if (std::getline(file, line)) {
            std::stringstream ss(line);
            char ids[ACCOUNTNAME_LENGTH] = {0};
            int index, eff1, val1, eff2, val2, eff3, val3;

            ss >> ids >> index >> eff1 >> val1 >> eff2 >> val2 >> eff3 >> val3;
            file.close();
            
            auto& db = m_server.getDatabase();
            auto accountOpt = db.readAccount(ids);
            if (accountOpt) {
                STRUCT_ACCOUNTFILE acc = *accountOpt;
                
                STRUCT_ITEM newItem{};
                newItem.sIndex = index;
                newItem.stEffect[0].cEffect = eff1;
                newItem.stEffect[0].cValue = val1;
                newItem.stEffect[1].cEffect = eff2;
                newItem.stEffect[1].cValue = val2;
                newItem.stEffect[2].cEffect = eff3;
                newItem.stEffect[2].cValue = val3;
                
                bool placed = false;
                for(int i = 0; i < MAX_CARGO; i++) {
                    if (acc.Cargo[i].sIndex == 0) {
                        acc.Cargo[i] = newItem;
                        placed = true;
                        break;
                    }
                }

                if(placed) {
                    db.writeAccount(acc);
                    Logger::Log("Item importado para a conta " + std::string(ids), "DataManager");
                } else {
                     Logger::Log("Nao ha espaco no bau da conta " + std::string(ids), "DataManager", true);
                }
                
                try { std::filesystem::remove(entry.path()); }
                catch(...){}
            } else {
                 try { 
                    std::filesystem::rename(entry.path(), "../../Common/Error/" + entry.path().filename().string());
                 } catch(...){}
            }
        } else {
            file.close();
        }
    }
}
// As implementações de importDonates, importUsers, updateUserPasswords seguem o mesmo padrão
// ...

void DataManager::generateDailyLogs() {
    generateItemLog();
    generateExpLog();
}

void DataManager::generateItemLog() {
    // ... Implementação para criar o log de itens do dia.
}

void DataManager::generateExpLog() {
    // ... Implementação para criar o log de EXP do dia.
}