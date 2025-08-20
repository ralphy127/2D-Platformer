#pragma once

#include <string>
#include <vector>
#include <Utils/i2v.h>
#include <Utils/Logging.h>
#include "Engine/ISettingsObserver.h"

namespace engine {

/// @brief Manages game configuration settings and notifies registered observers on any changes.
class Settings {
public:
    /// @brief Configuration struct used for initializing Settings.
    struct Config {
        bool fullscreen;          ///< Whether the game runs in fullscreen mode.
        std::string title;        ///< Window title string.
        float targetFps;          ///< Target frames per second.
        bool showHitboxes;        ///< Whether to draw hitboxes for debugging.
        bool showTextureHitboxes; ///< Whether to draw texture hitboxes for debugging.
        utils::i2v windowSize;    ///< Logical resolution of the window.
        utils::i2v windowPos;     ///< Initial position of the window on screen.
        size_t tileSize;          ///< Size of a single tile in pixels.
    };

    Settings(const Config& config);
    ~Settings() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Settings destroyed"); }

    void registerObserver(ISettingsObserver& observer) { _observers.push_back(&observer); }
    void unregisterObserver(ISettingsObserver& observer);

    bool isFullscreen() const { return _fullscreen; }
    void setFullscreen(bool fullscreen) { _fullscreen = fullscreen; notifyObservers(); }

    const std::string& getTitle() const { return _title; }

    float getTargetFps() const { return _targetFps; }
    void setTargetFps(float targetFps) { _targetFps = targetFps; notifyObservers(); }

    bool showHitboxes() const { return _showHitboxes; }
    void setHitboxes(bool showHitboxes) { _showHitboxes = showHitboxes; notifyObservers(); }

    bool showTextureHitboxes() const { return _showTextureHitboxes; }
    void setTextureHitboxes(bool showTextureHitboxes) {
        _showTextureHitboxes = showTextureHitboxes; notifyObservers(); }

    utils::i2v getWindowSize() const { return _windowSize; }
    void setWindowSize(utils::i2v windowSize) { _windowSize = windowSize; notifyObservers(); }

    utils::i2v getWindowPos() const { return _windowPos; }

    size_t getTileSize() const { return _tileSize; }
    void setTileSize(size_t tileSize) { _tileSize = tileSize; notifyObservers(); }

private:
    /// @brief Notifies all registered observers about a settings change.
    void notifyObservers();

    std::vector<ISettingsObserver*> _observers{}; ///< Registered observers for setting changes.

    bool _fullscreen;          ///< True if fullscreen is enabled.
    std::string _title;        ///< Window title.
    float _targetFps;          ///< Target frames per second.
    bool _showHitboxes;        ///< Whether to render debug hitboxes.
    bool _showTextureHitboxes; ///< Whether to render debug texture hitboxes.
    utils::i2v _windowPos;     ///< Window position on screen.
    utils::i2v _windowSize;    ///< Logical window resolution.
    size_t _tileSize;          ///< Size of one tile in pixels.
};

}
