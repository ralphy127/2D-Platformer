#pragma once

#include <memory>

#include <Utils/SDLUtils.h>
#include <Utils/i2v.h>
#include <Utils/Logging.h>
#include "Engine/Settings.h"

namespace engine {

/// @brief Manages SDL window and renderer lifecycle and properties
class WindowManager : public ISettingsObserver {
public:
    /// @brief Constructs a WindowManager, initializing SDL window and renderer.
    /// @param settings Game settings to get window size and fullscreen configuration
    WindowManager(Settings&);

    ~WindowManager();

    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
    WindowManager(WindowManager&&) = delete;
    WindowManager& operator=(WindowManager&&) = delete;

    /// @brief Gets the raw pointer to the SDL window.
    /// @return Pointer to SDL_Window.
    SDL_Window& getWindow() { return *_window; }

    /// @brief Gets the current window size.
    /// @return Window size as an i2v struct.
    utils::i2v getWindowSize() const { return _windowSize; }

    /// @brief Sets a new window size.
    /// @param windowSize The new window size as an i2v struct.
    void setWindowSize(const utils::i2v& windowSize);

    /// @brief Set fullscreen configuration
    /// @param fullscreen If true, display mode is fulscreen
    void setFullscreen(bool fullscreen) { _fullscreen = fullscreen; }

    void onSettingsChanged() override;
private:
    /// @brief Initializes window dimensions based on configuration or fullscreen display mode.
    void initFullScreenWindowDimensions();
    void resetWindow(const std::string& title, int x, int y, int w, int h, Uint32 flags) {
        _window.reset(SDL_CreateWindow(title.c_str(), x, y, w, h, flags));

        if(!_window)
            throw std::runtime_error("Failed to create SDL Window: " + std::string(SDL_GetError()));
    }

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window; ///< Managed SDL window pointer

    Settings& _settings;
    bool _fullscreen; ///< If true, the game runs in fullscreen mode.
    utils::i2v _windowSize;  ///< Current window size
};

}
