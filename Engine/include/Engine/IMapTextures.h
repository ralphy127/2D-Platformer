#pragma once

#include <SDL.h>

namespace engine {

using MapTextureType = uint16_t;
using MapTextureId = uint16_t;

/// @brief Interface for map textures provider.
class IMapTextures {
public:
    virtual ~IMapTextures() = default;

    /// @brief Gets texture based on type and identifier.
    virtual SDL_Texture& getTexture(MapTextureType, MapTextureId) = 0;
};

}
