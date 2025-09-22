#pragma once
#include "../common.h"

class Server; // Forward declaration

class TaskManager {
public:
    TaskManager(Server& server);
    ~TaskManager();

    void start();
    void stop();

private:
    void run();
    
    Server& m_server;
    std::atomic<bool> m_isRunning{false};
    std::thread m_thread;
};
