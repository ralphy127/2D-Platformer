#include "Engine/Clock.h"

#include <thread>

namespace engine {

Clock::Clock(Settings& settings) 
    : _settings(settings)
{   
    onSettingsChanged();

    _settings.registerObserver(*this);

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Clock created");
}

void Clock::tickAndWait() {
    using namespace std::chrono;

    _now = ClockType::now();
    auto frameDuration = _now - _lastFrameTime;

    if (frameDuration < _targetFrameDuration) {
        auto sleepTime = _targetFrameDuration - frameDuration;

        auto activeWait = 0.9f;
        auto safeSleepTime = duration_cast<milliseconds>(sleepTime * activeWait);
        if (safeSleepTime.count() > 0LL)
            std::this_thread::sleep_for(safeSleepTime);
    }

    auto currentTime = ClockType::now();
    _deltaTime = duration<float>(currentTime - _lastFrameTime).count();
    _lastFrameTime = currentTime;

    _actualFps = 1.0f / _deltaTime;
}

void Clock::onSettingsChanged() {
    _targetFps = _settings.getTargetFps();

    std::chrono::duration<float> secondsPerFrame(1.0f / _targetFps);
    
    _targetFrameDuration = std::chrono::duration_cast<ClockType::duration>(secondsPerFrame);
}

}
