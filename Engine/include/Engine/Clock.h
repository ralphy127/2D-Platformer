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

    /// @brief Constructs Clock and initializes timing variables based on settings.
    /// @param settings Reference to the settings object (used for target FPS, etc.).
    Clock(Settings&);

    ~Clock() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Clock destroyed"); }

    Clock(const Clock&) = delete;
    Clock& operator=(const Clock&) = delete;
    Clock(Clock&&) = delete;
    Clock& operator=(Clock&&) = delete;

    /// @brief Regulates frame rate to maintain consistent timing and updates delta time.
    void tickAndWait();

    /// @brief Returns time elapsed since the last frame in seconds.
    /// @return Delta time in seconds.
    float getDeltaTime() const { return _deltaTime; }

    /// @brief Returns the actual measured frames per second.
    /// @return Current frames per second.
    float getFps() const { return _actualFps; }

    /// @brief Responds to settings changes, e.g. updates target FPS.
    void onSettingsChanged() override;

    /// @brief Returns current high-resolution clock timestamp.
    /// @return Current time point.
    ClockType::time_point getTime() { return _now; }

private:
    Settings& _settings;                          ///< Reference to settings.

    float _deltaTime{};                           ///< Time elapsed between frames in seconds.
    ClockType::time_point _lastFrameTime{ClockType::now()}; ///< Timestamp of last frame.
    float _targetFps{};                           ///< Target frames per second.
    ClockType::duration _targetFrameDuration{};   ///< Target duration of a single frame.
    float _actualFps{};                           ///< Calculated actual FPS.
    ClockType::time_point _now{ClockType::now()}; ///< timestamp of current frame
};

}
