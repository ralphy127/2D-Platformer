#pragma once

#include <cstddef>
#include <SDL_mixer.h>
#include <Utils/Logging.h>

namespace engine {

/// @brief Manages SDL initialization, subsystems, audio and cleanup.
class SDLManager {
public:
    SDLManager();
    ~SDLManager();

    void setVolume(size_t volume);

private:
    size_t _volume{10UL}; ///< Current volume level
};

}
