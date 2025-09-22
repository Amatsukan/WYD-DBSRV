#pragma once
// Includes Padrão do C++
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <atomic>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <optional>
#include <algorithm>
// Includes do Windows e Sockets
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
// Includes do Projeto Antigo (mantidos para as structs)
#include "Basedef.h"
#include "CPSock.h"
#include "ItemEffect.h"
// Forward declarations de classes legadas que viraram structs de dados
class CUser;
// Constantes
constexpr int SERVER_TICK_RATE_MS = 1000;
