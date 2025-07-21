#include "Engine/SDLManager.h"

#include <stdexcept>
#include <string>
#include <SDL_ttf.h>

namespace engine {

SDLManager::SDLManager() {
    if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_SENSOR ))
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + std::string(SDL_GetError()));

    if(TTF_Init() < 0)
        throw std::runtime_error(std::string("Failed to initialize SDL fonts: ") + std::string(TTF_GetError()));

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        throw std::runtime_error(std::string("Failed to initialize SDL mixes: ") + std::string(Mix_GetError()));

    Mix_Volume(-1, static_cast<int>(_volume / 10. * SDL_MIX_MAXVOLUME));
    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "SDL manager created");
}

SDLManager::~SDLManager() {
    SDL_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "SDL manager destroyed");
}

}
