#pragma once

#include <string>
#include <vector>
#include <Utils/i2v.h>
#include <Utils/Logging.h>
#include "Engine/ISettingsObserver.h"

namespace engine {

/// @brief Stores global configuration settings for the game.
class Settings {
public:
    /// @brief Constructs default settings.
    Settings() { SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Settings created"); }

    ~Settings() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Settings destroyed"); }

    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(Settings&&) = delete;
    Settings& operator=(const Settings&) = delete;

    void registerObserver(ISettingsObserver& observer) { _observers.push_back(&observer); }
    void unregisterObserver(ISettingsObserver& observer);
     
    bool isFullscreen() const { return _fullscreen; }
    void setFullscreen(bool fullscreen) { _fullscreen = fullscreen; notifyObservers(); }

    const std::string& getTitle() const { return _title; }

    float getTargetFps() const { return _targetFps; }

    /// @brief Sets target FPS and recalculates frame time accordingly.
    void setTargetFps(float targetFps) { _targetFps = targetFps; notifyObservers(); }

    bool showHitboxes() const { return _showHitboxes; }
    void setHitboxes(bool showHitboxes) { _showHitboxes = showHitboxes; notifyObservers(); }

    bool showTextureHitboxes() const { return _showTextureHitboxes; }
    void setTextureHitboxes(bool showTextureHitboxes) { _showTextureHitboxes = showTextureHitboxes; notifyObservers(); }

    utils::i2v getWindowSize() const { return _windowSize; }
    void setWindowSize(utils::i2v windowSize) { _windowSize = windowSize; notifyObservers(); }

    utils::i2v getWindowPos() const { return _windowPos; }

    size_t getTileSize() const { return _tileSize; }
    void setTileSize(size_t tileSize) { _tileSize = tileSize; notifyObservers(); }
private:
    std::vector<ISettingsObserver*> _observers{};
    void notifyObservers();

    bool _fullscreen{true};            ///< If true, the game runs in fullscreen mode.
    std::string _title{std::string("2D Platformer")};          ///< Window title.
    float _targetFps{60.f};            ///< Target frames per second.
    bool _showHitboxes{true};          ///< Whether to draw hitboxes for debugging.
    bool _showTextureHitboxes{true};

    utils::i2v _windowSize{720, 460};             ///< Logical window resolution.
    utils::i2v _windowPos{SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};
    size_t _tileSize{0UL};            ///< Size of a single tile in pixels.
};

}
