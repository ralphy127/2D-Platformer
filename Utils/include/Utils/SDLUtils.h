#pragma once

#include <memory>
#include <SDL.h>

namespace utils {

/// @brief Utility class for managing SDL_Surface and SDL_Texture with smart pointers.
class SDLUtils {
public:
    /// @brief Type alias for a unique pointer managing SDL_Texture with SDL_DestroyTexture deleter.
    using TexturePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
    
    /// @brief Type alias for a unique pointer managing SDL_Surface with SDL_FreeSurface deleter.
    using SurfacePtr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;

    /// @brief Wraps a raw SDL_Surface pointer into a managed SurfacePtr.
    /// @param raw Raw pointer to SDL_Surface.
    /// @return SurfacePtr managing the SDL_Surface with SDL_FreeSurface deleter.
    static SurfacePtr makeSurface(SDL_Surface* raw) { return SurfacePtr(raw, SDL_FreeSurface); }

    /// @brief Wraps a raw SDL_Texture pointer into a managed TexturePtr.
    /// @param raw Raw pointer to SDL_Texture.
    /// @return TexturePtr managing the SDL_Texture with SDL_DestroyTexture deleter.
    static TexturePtr makeTexture(SDL_Texture* raw) { return TexturePtr(raw, SDL_DestroyTexture); }
};

}
