#pragma once

#include <chrono>
#include <SDL.h>
#include <Utils/Logging.h>
#include "Engine/Settings.h"

namespace engine {

/// @brief Manages frame timing, delta time calculation, and frame rate regulation.
class Clock : ISettingsObserver {
public:
    using ClockType = std::chrono::high_resolution_clock;

    /// @brief Constructs Clock and initializes timing variables.
    Clock(Settings&);

    ~Clock() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Clock destroyed"); }

    Clock(const Clock&) = delete;
    Clock& operator=(const Clock&) = delete;
    Clock(Clock&&) = delete;
    Clock& operator=(Clock&&) = delete;

    /// @brief Regulates frame rate and updates delta time.
    void tickAndWait();

    /// @brief Returns time elapsed since last frame in seconds.
    /// @return Delta time in seconds.
    float getDeltaTime() const { return _deltaTime; }

    /// @brief Returns actual fps
    /// @return Current fps [1/s]
    float getFps() const { return _actualFps; }

    void onSettingsChanged() override;

    static ClockType::time_point getTime() { return _now; }
private:
    /// @brief Logs the current FPS to the debug output.
    void logFPS();

    Settings& _settings;

    float _deltaTime{};                                     ///< Delta time between frames (seconds).
    ClockType::time_point _lastFrameTime{ClockType::now()}; ///< Timestamp of last frame (milliseconds).
    float _targetFps;                                       ///< Target frames per second.
    ClockType::duration _targetFrameDuration;               ///< Target frame duration (milliseconds).
    float _actualFps{};

    inline static ClockType::time_point _now{ClockType::now()};

    /// Time of last fps log
    inline static ClockType::time_point _lastLogTime = ClockType::now();
    /// The delay between FPS log messages in seconds
    static constexpr ClockType::duration FPS_LOG_DELAY = std::chrono::seconds(1LL);
};

}
