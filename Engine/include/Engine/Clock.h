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

    float getDeltaTime() const { return _deltaTime; }

    float getFps() const { return _actualFps; }

    void onSettingsChanged() override;

    static Type::time_point getTime() { return _now; }

private:
    Settings& _settings;                              ///< Reference to settings.

    float _deltaTime{};                               ///< Time elapsed between frames [s].
    Type::time_point _lastFrameTime{Type::now()};     ///< Timestamp of last frame.
    float _targetFps{};                               ///< Target frames per second.
    Type::duration _targetFrameDuration{};            ///< Target duration of a single frame.
    float _actualFps{};                               ///< Calculated actual FPS.
    inline static Type::time_point _now{Type::now()}; ///< timestamp of current frame.
};

}
