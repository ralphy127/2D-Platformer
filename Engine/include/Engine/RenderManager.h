#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <Utils/Logging.h>

namespace engine {

class RenderManager {
public:
    RenderManager(SDL_Window&);
    ~RenderManager() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Render manager destroyed"); }

    SDL_Renderer& getRenderer() { return *_renderer; }

    /// @brief Clears the current rendering target with a fixed background color.
    void clear() const { SDL_RenderClear(_renderer.get()); }

    /// @brief Presents the rendered content on the screen.
    void present() const { SDL_RenderPresent(_renderer.get()); }

private:
    /// @brief Managed SDL renderer pointer
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>
        _renderer{nullptr, SDL_DestroyRenderer};
};

}
