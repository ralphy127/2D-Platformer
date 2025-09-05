#include "Engine/RenderManager.h"

#include "Engine/Colors.h"

namespace engine {

RenderManager::RenderManager(SDL_Window& window) {
    _renderer.reset(
        SDL_CreateRenderer(&window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

    if(!_renderer)
        throw std::runtime_error("Failed to create SDL Renderer: " + std::string(SDL_GetError()));

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Render manager created");
}

}
