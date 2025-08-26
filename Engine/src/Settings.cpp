#include "Engine/Settings.h"

namespace engine {

Settings::Settings(const Config& config)
    : _fullscreen(config.fullscreen),
      _title(std::move(config.title)),
      _targetFps(config.targetFps),
      _showHitboxes(config.showHitboxes),
      _showTextureHitboxes(config.showTextureHitboxes),
      _windowPos(config.windowPos),
      _windowSize(config.windowSize),
      _tileSize(config.tileSize) {
    
    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Settings created (custom)");
}

void Settings::unregisterObserver(ISettingsObserver& observer) {
    _observers.erase(
        std::remove(_observers.begin(), _observers.end(), &observer),
        _observers.end()
    );
}

void Settings::notifyObservers() {
    for (auto* observer : _observers) {
        if(!observer)
            throw std::logic_error("Trying to acccess observer which does not exist");

        observer->onSettingsChanged();
    }
}

}
