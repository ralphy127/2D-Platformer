#pragma once

#include <string>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <SDL.h>

namespace utils {

enum {
    LOG_CATEGORY_SETUP = SDL_LOG_CATEGORY_CUSTOM,
    LOG_CATEGORY_CLEANUP,
};

/**
 * @brief Custom SDL logger function that formats and outputs log messages to the console.
 * 
 * Log output format example:
 * [YYYY-MM-DD HH:MM:SS] [CATEGORY] PRIORITY: message
 * 
 * @param userData    Pointer to user data (unused).
 * @param category    SDL log category (e.g., SDL_LOG_CATEGORY_APPLICATION).
 * @param priority    SDL log priority level (e.g., SDL_LOG_PRIORITY_ERROR).
 * @param message     The log message string.
 */
void consoleSDLLogger(void* userData, int category, SDL_LogPriority, const char* message);

class Logger {
public:
    using ClockType = std::chrono::steady_clock;

    static void logDebugEvery(
        const std::string& key,
        ClockType::duration interval,
        const std::function<std::string()>& messageFunc);

    static bool shouldLog(const std::string& key, ClockType::duration interval);

private:
    static inline std::unordered_map<std::string, ClockType::time_point> _lastLogTimes{};
};

}
