#include "Utils/Logging.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <string>
#include <iomanip>

namespace utils {

void consoleSDLLogger(void* userData, int category, SDL_LogPriority priority, const char* message) {
    auto now_clock = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now_clock);
    auto* now = std::localtime(&now_time);
    
    std::ostringstream timestamp;
    timestamp << std::put_time(now, "%Y-%m-%d %H:%M:%S");

    std::string categoryStr;
    switch (category) {
        case SDL_LOG_CATEGORY_APPLICATION:
            categoryStr = "APP";
            break;
        case SDL_LOG_CATEGORY_ERROR:
            categoryStr = "ERROR";
            break;
        case SDL_LOG_CATEGORY_SYSTEM:
            categoryStr = "SYSTEM";
            break;
        case SDL_LOG_CATEGORY_RENDER:
            categoryStr = "RENDER";
            break;
        case SDL_LOG_CATEGORY_AUDIO:
            categoryStr = "AUDIO";
            break;
        case SDL_LOG_CATEGORY_INPUT:
            categoryStr = "INPUT";
            break;
        case SDL_LOG_CATEGORY_TEST:
            categoryStr = "TEST";
            break;
        case LOG_CATEGORY_SETUP:
            categoryStr = "SETUP";
            break;
        case LOG_CATEGORY_CLEANUP:
            categoryStr = "CLEANUP";
            break;
        default:
            categoryStr = "OTHER";
            break;
    }

    std::string priorityStr;
    std::string colorCode;
    switch (priority) {
        case SDL_LOG_PRIORITY_CRITICAL:
            priorityStr = "CRITICAL";
            colorCode = "\033[1;31m";
            break;
        case SDL_LOG_PRIORITY_ERROR:
            priorityStr = "ERROR";
            colorCode = "\033[38;5;208m";
            break;
        case SDL_LOG_PRIORITY_WARN:
            priorityStr = "WARN";
            colorCode = "\033[1;33m";
            break;
        case SDL_LOG_PRIORITY_INFO:
            priorityStr = "INFO";
            colorCode = "\033[1;34m";
            break;
        case SDL_LOG_PRIORITY_DEBUG:
            priorityStr = "DEBUG";
            colorCode = "\033[0;37m";
            break;
        case SDL_LOG_PRIORITY_VERBOSE:
            priorityStr = "VERBOSE";
            colorCode = "\033[1;30m";
            break;
        default:
            priorityStr = "UNKNOWN";
            colorCode = "\033[0m";
            break;
    }

    const int categoryWidth = 12;
    const int priotityWidth = 8;

    std::cout << "[" << timestamp.str() << "] "
              << std::left << std::setw(categoryWidth) << "[" + categoryStr + "] "
              << colorCode << std::left << std::setw(priotityWidth)
              << priorityStr 
              << "\033[0m" << " : " << message
              << std::endl;
}

void Logger::logDebugEvery(
    const std::string& key,
    ClockType::duration interval,
    const std::string& message) {

    if (!shouldLog(key, interval)) {
        return;
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[%s] %s", key.c_str(), message.c_str());

    _lastLogTimes[key] = ClockType::now();
}

bool Logger::shouldLog(const std::string& key, ClockType::duration interval) {
    const auto now = ClockType::now();
    const auto [it, inserted] = _lastLogTimes.try_emplace(key, now);

    return inserted || now - it->second >= interval;
}

}