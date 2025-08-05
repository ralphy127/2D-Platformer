#pragma once

#include <SDL.h>
#include <unordered_map>
#include <Utils/Logging.h>

namespace engine {

/// @brief Handles user input events using SDL.
class EventHandler {
public:
    EventHandler() { SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Event handler created"); }

    ~EventHandler() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Event handler destroyed"); }
    
    /// @brief Polls SDL events and updates internal key state map.
    /// Should be called once per frame to process new input.
    void pollEvents();

    /// @brief Returns a read-only reference to the map of key states.
    /// @return A map where keys are SDL_Keycodes and values indicate if the key is pressed.
    const std::unordered_map<SDL_Keycode, bool>& getKeyStates() const { return _keyStates; }
    
    /// @brief Clears the internal key state map.
    /// Useful to reset key states after processing them in a frame.
    void resetKeyStates();

    /// @brief Checks if a specific key is currently marked as pressed.
    /// @param key The SDL_Keycode of the key to check.
    /// @return True if the key is pressed, false if not pressed or not present in the map.
    bool isKeyPressed(SDL_Keycode) const;

    /// @brief Checks if any of the specified keys is currently pressed.
    /// @tparam Keys Variadic template parameter pack of SDL_Keycode keys.
    /// @param keys The keys to check.
    /// @return True if at least one of the specified keys is pressed, false otherwise.
    template<typename... Keys>
    bool isAnyKeyPressed(Keys... keys) const { return (... || isKeyPressed(keys)); }

    /// @brief Checks if quit is requested
    /// @return True if SDL_QUIT event is raised
    bool isQuitRequested() const { return _quitRequested; }

private:
    /// @brief Internal map storing the state of keys.
    /// True means pressed, false means not pressed.
    std::unordered_map<SDL_Keycode, bool> _keyStates{};

    bool _quitRequested{false}; ///< True if SDL_QUIT event is raised
};

}
