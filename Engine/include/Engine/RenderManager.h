#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <Utils/Logging.h>

namespace engine {

/// @brief Manages clearing and presenting the SDL renderer.
class RenderManager {
public:
    /// @brief Constructs RenderManager with the given SDL renderer.
    /// @param renderer Pointer to the SDL_Renderer to manage.
    RenderManager(SDL_Window&);

    ~RenderManager() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Render manager destroyed"); }

    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;
    RenderManager(RenderManager&&) = default;
    RenderManager& operator=(RenderManager&&) = default;

    /// @brief Gets the raw pointer to the SDL renderer.
    /// @return Pointer to SDL_Renderer.
    SDL_Renderer& getRenderer() { return *_renderer; }

    /// @brief Clears the current rendering target with a fixed background color.
    void clear() const { SDL_SetRenderDrawColor(_renderer.get(), 50, 200, 200, 255); SDL_RenderClear(_renderer.get()); }

    /// @brief Presents the rendered content on the screen.
    void present() const { SDL_RenderPresent(_renderer.get()); }

private:
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer{nullptr, SDL_DestroyRenderer}; ///< Managed SDL renderer pointer
};

}
