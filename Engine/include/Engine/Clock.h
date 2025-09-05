#pragma once

#include <chrono>
#include <SDL.h>
#include <Utils/Logging.h>
#include "Engine/Settings.h"

namespace engine {

class Clock : ISettingsObserver {
public:
    using Type = std::chrono::high_resolution_clock;

    Clock(Settings&);
    ~Clock() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Clock destroyed"); }

    /// @brief Regulates frame rate to maintain consistent timing and updates delta time.
    void tickAndWait();

    float32_t getDeltaTime() const { return _deltaTime; }

    float32_t getFps() const { return _actualFps; }

    void onSettingsChanged() override;

    static Type::time_point getTime() { return _now; }

private:
    Settings& _settings;                              ///< Reference to settings.

    float32_t _deltaTime{};                               ///< Time elapsed between frames [s].
    Type::time_point _lastFrameTime{Type::now()};     ///< Timestamp of last frame.
    float32_t _targetFps{};                               ///< Target frames per second.
    Type::duration _targetFrameDuration{};            ///< Target duration of a single frame.
    float32_t _actualFps{};                               ///< Calculated actual FPS.
    inline static Type::time_point _now{Type::now()}; ///< timestamp of current frame.
};

}
