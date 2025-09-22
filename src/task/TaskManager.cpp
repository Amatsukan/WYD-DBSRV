#include "TaskManager.h"
#include "../Server.h" // Precisa da definição completa do Server
#include "../log/Logger.h"

TaskManager::TaskManager(Server& server) : m_server(server) {}

TaskManager::~TaskManager() {
    stop();
}

void TaskManager::start() {
    m_isRunning = true;
    m_thread = std::thread(&TaskManager::run, this);
    Logger::Log("TaskManager iniciado.", "Tasks");
}

void TaskManager::stop() {
    m_isRunning = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void TaskManager::run() {
    while (m_isRunning) {
        // Chama a função de tick do servidor
        m_server.processTick();

        // Dorme pelo tempo definido
        std::this_thread::sleep_for(std::chrono::milliseconds(SERVER_TICK_RATE_MS));
    }
}
