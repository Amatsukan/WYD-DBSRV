#pragma once
#include "../common.h"

class Logger {
public:
    static void Init(const std::string& logFileName);
    static void Shutdown();
    static void Log(const std::string& message, const std::string& category = "system", bool isError = false);

private:
    static std::ofstream m_logFile;
    static std::mutex m_mutex;
};
