#include "Logger.h"

std::ofstream Logger::m_logFile;
std::mutex Logger::m_mutex;

void Logger::Init(const std::string& logFileName) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logFile.open(logFileName, std::ios::out | std::ios::app);
    if (!m_logFile.is_open()) {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo de log: " << logFileName << std::endl;
    }
}

void Logger::Shutdown() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void Logger::Log(const std::string& message, const std::string& category) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
	time_t tt = time(0);
	tm* localtm = localtime(&tt);
    ss << std::put_time(localtm, "%Y-%m-%d %X");
    
    std::string logMessage = "[" + ss.str() + "] [" + category + "] " + message;
    
    std::lock_guard<std::mutex> lock(m_mutex);
    std::cout << logMessage << std::endl;
    if (m_logFile.is_open()) {
        m_logFile << logMessage << std::endl;
		m_logFile.flush();
    }
}
