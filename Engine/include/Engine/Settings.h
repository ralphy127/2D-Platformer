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
        bool fullscreen;                 ///< Whether the game runs in fullscreen mode.
        std::string title;               ///< Window title string.
        float targetFps;                 ///< Target frames per second.
        bool showHitboxes;               ///< Whether to draw hitboxes for debugging.
        bool showTextureHitboxes;        ///< Whether to draw texture hitboxes for debugging.
        utils::i2v windowSize;           ///< Logical resolution of the window.
        utils::i2v windowPos;            ///< Initial position of the window on screen.
        size_t tileSize;                 ///< Size of a single tile in pixels.
    };

    /// @brief Constructs Settings with the given configuration.
    /// @param config Initial configuration to move into the Settings object.
    Settings(Config&& config);

    /// @brief Destructor logs the cleanup event.
    ~Settings() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Settings destroyed"); }

    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(Settings&&) = delete;
    Settings& operator=(const Settings&) = delete;

    /// @brief Registers an observer to receive setting change notifications.
    /// @param observer Reference to the observer to register.
    void registerObserver(ISettingsObserver& observer) { _observers.push_back(&observer); }

    /// @brief Unregisters an observer, so it no longer receives setting change notifications.
    /// @param observer Reference to the observer to unregister.
    void unregisterObserver(ISettingsObserver& observer);

    /// @brief Checks if fullscreen mode is enabled.
    /// @return True if fullscreen mode is active.
    bool isFullscreen() const { return _fullscreen; }

    /// @brief Enables or disables fullscreen mode.
    /// @param fullscreen New fullscreen state.
    void setFullscreen(bool fullscreen) { _fullscreen = fullscreen; notifyObservers(); }

    /// @brief Gets the window title string.
    /// @return Constant reference to the window title.
    const std::string& getTitle() const { return _title; }

    /// @brief Gets the target frames per second.
    /// @return Target FPS value.
    float getTargetFps() const { return _targetFps; }

    /// @brief Sets the target frames per second.
    /// Notifies observers of the change.
    /// @param targetFps New target FPS.
    void setTargetFps(float targetFps) { _targetFps = targetFps; notifyObservers(); }

    /// @brief Returns whether hitboxes should be shown for debugging.
    /// @return True if hitboxes are shown.
    bool showHitboxes() const { return _showHitboxes; }

    /// @brief Enables or disables hitbox rendering.
    /// @param showHitboxes Whether to show hitboxes.
    void setHitboxes(bool showHitboxes) { _showHitboxes = showHitboxes; notifyObservers(); }

    /// @brief Returns whether texture hitboxes should be shown.
    /// @return True if texture hitboxes are shown.
    bool showTextureHitboxes() const { return _showTextureHitboxes; }

    /// @brief Enables or disables texture hitbox rendering.
    /// @param showTextureHitboxes Whether to show texture hitboxes.
    void setTextureHitboxes(bool showTextureHitboxes) {
        _showTextureHitboxes = showTextureHitboxes; notifyObservers(); }

    /// @brief Gets the logical window size.
    /// @return Current window resolution.
    utils::i2v getWindowSize() const { return _windowSize; }

    /// @brief Sets the logical window size.
    /// @param windowSize New window size.
    void setWindowSize(utils::i2v windowSize) { _windowSize = windowSize; notifyObservers(); }

    /// @brief Gets the window position on screen.
    /// @return Current window position.
    utils::i2v getWindowPos() const { return _windowPos; }

    /// @brief Gets the tile size in pixels.
    /// @return Size of one tile in pixels.
    size_t getTileSize() const { return _tileSize; }

    /// @brief Sets the tile size in pixels.
    /// @param tileSize New tile size.
    void setTileSize(size_t tileSize) { _tileSize = tileSize; notifyObservers(); }

private:
    /// @brief Notifies all registered observers about a settings change.
    void notifyObservers();

    std::vector<ISettingsObserver*> _observers{}; ///< Registered observers for setting changes.

    bool _fullscreen{true};                        ///< True if fullscreen is enabled.
    std::string _title{"2D Platformer"};           ///< Window title.
    float _targetFps{60.f};                        ///< Target frames per second.
    bool _showHitboxes{true};                      ///< Whether to render debug hitboxes.
    bool _showTextureHitboxes{true};               ///< Whether to render debug texture hitboxes.

    /// @brief Window position on screen.
    utils::i2v _windowPos{SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};
    utils::i2v _windowSize{720, 460};              ///< Logical window resolution.
    size_t _tileSize{0UL};                         ///< Size of one tile in pixels.
};

}
