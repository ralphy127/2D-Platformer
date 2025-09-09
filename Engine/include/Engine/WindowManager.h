#pragma once

#include <memory>

#include <Utils/SDLUtils.h>
#include <Utils/i2v.h>
#include <Utils/Logging.h>
#include "Engine/Settings.h"

namespace engine {

class WindowManager : public ISettingsObserver {
public:
    WindowManager(Settings&);
    ~WindowManager();

    SDL_Window& getWindow() { return *_window; }

    utils::i2v getWindowSize() const { return _windowSize; }
    void setWindowSize(const utils::i2v& windowSize);

    void setFullscreen(bool fullscreen) { _fullscreen = fullscreen; }

    void onSettingsChanged() override;
private:
    /// @brief Initializes window dimensions based on configuration.
    void initFullScreenWindowDimensions();

    /// @brief Resets the SDL window with the specified parameters.
    void resetWindow(const std::string& title, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t flags);
    
    Settings& _settings;    ///< Reference to game settings

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window; ///< Managed SDL window
    bool _fullscreen;       ///< If true, the game runs in fullscreen mode.
    utils::i2v _windowSize; ///< Current window size
};

}
