#include "DatabaseManager.h"
#include "../log/logger.h"
#include <cctype>
#include <algorithm>

// Função auxiliar para converter string para maiúsculas
std::string toUpper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return s;
}

std::optional<STRUCT_ACCOUNTFILE> DatabaseManager::readAccount(const std::string& accountName) {
    std::string path = getAccountPath(accountName);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return std::nullopt;
    }

    STRUCT_ACCOUNTFILE accFile{};
    file.read(reinterpret_cast<char*>(&accFile), sizeof(STRUCT_ACCOUNTFILE));
    
    // Se a leitura for incompleta (arquivo antigo/corrompido), preenche o resto com zeros
    if (file.gcount() < sizeof(STRUCT_ACCOUNTFILE)) {
        size_t bytesRead = file.gcount();
        memset(reinterpret_cast<char*>(&accFile) + bytesRead, 0, sizeof(STRUCT_ACCOUNTFILE) - bytesRead);
    }
    return accFile;
}

bool DatabaseManager::writeAccount(const STRUCT_ACCOUNTFILE& accountFile) {
    std::string path = getAccountPath(accountFile.Info.AccountName);
    
    try {
        std::filesystem::path dirPath = std::filesystem::path(path).parent_path();
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directories(dirPath);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        Logger::Log("Erro ao criar diretorio para conta: " + std::string(e.what()), "Database", true);
        return false;
    }

    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        Logger::Log("ERRO: Nao foi possivel escrever no arquivo da conta: " + path, "Database", true);
        return false;
    }

    file.write(reinterpret_cast<const char*>(&accountFile), sizeof(STRUCT_ACCOUNTFILE));
    return true;
}

bool DatabaseManager::accountExists(const std::string& accountName) {
    return std::filesystem::exists(getAccountPath(accountName));
}

bool DatabaseManager::createCharacter(const std::string& accountName, const std::string& charName) {
    std::string path = getCharPath(charName);
    if (std::filesystem::exists(path)) {
        return false; // Personagem já existe
    }
    
    try {
        std::filesystem::path dirPath = std::filesystem::path(path).parent_path();
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directories(dirPath);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        Logger::Log("Erro ao criar diretorio para personagem: " + std::string(e.what()), "Database", true);
        return false;
    }

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    char accNameBuffer[ACCOUNTNAME_LENGTH] = {0};
    strncpy_s(accNameBuffer, accountName.c_str(), _TRUNCATE);
    file.write(accNameBuffer, sizeof(accNameBuffer));
    
    return true;
}

bool DatabaseManager::deleteCharacter(const std::string& charName) {
    try {
        return std::filesystem::remove(getCharPath(charName));
    } catch (const std::filesystem::filesystem_error& e) {
        Logger::Log("Erro ao deletar personagem: " + std::string(e.what()), "Database", true);
        return false;
    }
}

std::string DatabaseManager::getAccountByChar(const std::string& charName) {
    std::string path = getCharPath(charName);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return "";
    }
    char accNameBuffer[ACCOUNTNAME_LENGTH] = {0};
    file.read(accNameBuffer, sizeof(accNameBuffer));
    return std::string(accNameBuffer);
}

std::vector<std::string> DatabaseManager::getAllAccountFiles() {
    std::vector<std::string> files;
    std::string root = "./account";
    if (!std::filesystem::exists(root)) return files;

    try {
        for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(root)) {
            if (dir_entry.is_regular_file()) {
                files.push_back(dir_entry.path().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        Logger::Log("Erro ao listar arquivos de conta: " + std::string(e.what()), "Database", true);
    }
    return files;
}

std::string DatabaseManager::getAccountPath(const std::string& accountName) {
    std::string upperAccount = toUpper(accountName);
    // Sanitiza para remover caracteres inválidos em nomes de arquivo
    upperAccount.erase(std::remove_if(upperAccount.begin(), upperAccount.end(), [](char c) { return c == '\0'; }), upperAccount.end());
    return "./account/" + getFirstKey(upperAccount) + "/" + upperAccount;
}

std::string DatabaseManager::getCharPath(const std::string& charName) {
    std::string upperChar = toUpper(charName);
    upperChar.erase(std::remove_if(upperChar.begin(), upperChar.end(), [](char c) { return c == '\0'; }), upperChar.end());
    return "./char/" + getFirstKey(upperChar) + "/" + upperChar;
}

std::string DatabaseManager::getFirstKey(const std::string& source) {
    if (source.empty()) return "etc";
    char firstChar = source[0];
    if (isalpha(static_cast<unsigned char>(firstChar))) {
        return std::string(1, std::tolower(static_cast<unsigned char>(firstChar)));
    }
    return "etc";
}