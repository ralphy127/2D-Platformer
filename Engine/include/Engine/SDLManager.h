#pragma once

#include <cstddef>
#include <SDL_mixer.h>
#include <Utils/Logging.h>

namespace engine {

using Volume = uint8_t;

/// @brief Manages SDL initialization, subsystems, audio and cleanup.
class SDLManager {
public:
    SDLManager();
    ~SDLManager();

    void setVolume(Volume);

private:
    Volume _volume{10}; ///< Current volume level
};

}
