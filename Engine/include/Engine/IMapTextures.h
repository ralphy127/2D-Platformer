#pragma once

#include <SDL.h>

namespace engine {

/// @brief Interface for map textures provider.
class IMapTextures {
public:
    virtual ~IMapTextures() = default;

    /// @brief Gets texture based on type and identifier.
    /// @param type Texture type identifier.
    /// @param id Texture index or id.
    /// @return Reference to SDL_Texture.
    virtual SDL_Texture& getTexture(int type, size_t id) = 0;
};

}
