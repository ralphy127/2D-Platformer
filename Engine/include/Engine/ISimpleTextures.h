#pragma once

#include <SDL.h>
#include "Engine/Entity.h"

namespace engine {

/// @brief Interface for simple textures provider (for non-animated entities).
class ISimpleTextures {
public:
    virtual ~ISimpleTextures() = default;

    /// @brief Retrieves the SDL_Texture associated with a simple entity type.
    /// @param type The type of the entity.
    /// @return Reference to the SDL_Texture corresponding to the entity type.
    virtual SDL_Texture& getTexture(Entity::Type) = 0;
};

}
