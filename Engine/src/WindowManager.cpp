#include "Engine/WindowManager.h"

#include <stdexcept>
#include <string>

namespace engine {

WindowManager::WindowManager(Settings& settings) 
    : _window(nullptr, SDL_DestroyWindow),
      _settings(settings) {

    _fullscreen = _settings.isFullscreen();
    if (_fullscreen) {
        initFullScreenWindowDimensions();
        _settings.setWindowSize(_windowSize);
    }
    else
        _windowSize = _settings.getWindowSize();
    
    auto flags = _fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;

    auto windowPos = _settings.getWindowPos();

    resetWindow(
        _settings.getTitle(),
        windowPos.x,
        windowPos.y,
        _windowSize.x,
        _windowSize.y,
        flags);

    _settings.registerObserver(*this);

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Window manager created");
}

WindowManager::~WindowManager() { 
    _settings.unregisterObserver(*this);

    SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Window manager destroyed");
}

void WindowManager::setWindowSize(const utils::i2v& windowSize) {
    if (windowSize < utils::i2v(0, 0)) throw std::invalid_argument("Window size must be positive");

    _windowSize = windowSize;
    SDL_SetWindowSize(_window.get(), windowSize.x, windowSize.y);
}

void WindowManager::onSettingsChanged() {
    auto oldFullScreen = _fullscreen;
    auto oldWindowSize = _windowSize;

    _fullscreen = _settings.isFullscreen();
    _windowSize = _settings.getWindowSize();

    if (_fullscreen != oldFullScreen && _fullscreen) {
        initFullScreenWindowDimensions();
        _settings.setWindowSize(_windowSize);
        auto windowPos = _settings.getWindowPos();
        resetWindow(
            _settings.getTitle(),
            windowPos.x,
            windowPos.y,
            _windowSize.x,
            _windowSize.y,
            SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else if(oldWindowSize != _windowSize) {
        auto windowPos = _settings.getWindowPos();
        resetWindow(
            _settings.getTitle(),
            windowPos.x,
            windowPos.y,
            _windowSize.x,
            _windowSize.y,
            0);
    }
}

void WindowManager::initFullScreenWindowDimensions() {
    SDL_DisplayMode displayMode;
    if(SDL_GetCurrentDisplayMode(0, &displayMode)) 
        throw std::runtime_error(std::string("Failed to initialize window dimensions: ")
                                 + SDL_GetError());
    
    if(displayMode.w <= 0 || displayMode.h <= 0)
        throw std::runtime_error(std::string("Failed to initialize window dimensions: ")
                                 + SDL_GetError());

    _windowSize = utils::i2v(displayMode.w, displayMode.h);
}

void WindowManager::resetWindow(
    const std::string& title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags) {

    _window.reset(SDL_CreateWindow(title.c_str(), x, y, w, h, flags));

    if(!_window)
        throw std::runtime_error("Failed to create SDL Window: " + std::string(SDL_GetError()));
}

}
