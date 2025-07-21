#pragma once

#include <cstddef>
#include <SDL_mixer.h>
#include <Utils/Logging.h>

namespace engine {

/// @brief Manages SDL initialization, audio and cleanup
class SDLManager {
public:
    /// @brief Constructs SDLManager and initializes SDL subsystems and audio
    SDLManager();

    /// @brief Cleans up SDL and audio subsystems
    ~SDLManager();

    SDLManager(const SDLManager&) = delete;
    SDLManager& operator=(const SDLManager&) = delete;
    SDLManager(SDLManager&&) = delete;
    SDLManager& operator=(SDLManager&&) = delete;

    /// @brief Sets the master volume level
    /// @param volume Volume level in range 0 to 10
    void setVolume(size_t volume) {
        _volume = volume > 10 ? 10 : volume;
        Mix_Volume(-1, static_cast<int>((_volume / 10.0) * SDL_MIX_MAXVOLUME));
    }

private:
    size_t _volume{10UL}; ///< Current volume level
};

}
