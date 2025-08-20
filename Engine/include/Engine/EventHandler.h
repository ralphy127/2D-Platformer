#pragma once

#include <SDL.h>
#include <unordered_map>
#include <Utils/Logging.h>

namespace engine {

class EventHandler {
public:
    EventHandler() { SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Event handler created"); }
    ~EventHandler() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Event handler destroyed"); }
    
    /// @brief Polls SDL events and updates internal key state map.
    /// Should be called once per frame to process new input.
    void pollEvents();

    const std::unordered_map<SDL_Keycode, bool>& getKeyStates() const { return _keyStates; }
    
    /// @brief Clears the internal key state map.
    void resetKeyStates();

    /// @brief Checks if a specific key is currently marked as pressed.
    bool isKeyPressed(SDL_Keycode) const;

    /// @brief Checks if any of the specified keys is currently pressed.
    template<typename... Keys>
    bool isAnyKeyPressed(Keys... keys) const { return (... || isKeyPressed(keys)); }

    bool isQuitRequested() const { return _quitRequested; }

private:
    /// @brief Internal map storing the state of keys.
    /// True means pressed, false means not pressed.
    std::unordered_map<SDL_Keycode, bool> _keyStates{};

    bool _quitRequested{}; ///< True if SDL_QUIT event is raised
};

}
