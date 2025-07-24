#pragma once

#include <SDL.h>
#include "Engine/Entity.h"

namespace engine {

/// @brief Interface providing textures for simple entities (non-animated).
class ISimpleTextures {
public:
    virtual ~ISimpleTextures() = default;

    /// @brief Retrieves the SDL_Texture associated with a simple entity type.
    /// @param type The type of the entity.
    /// @return Reference to the SDL_Texture corresponding to the entity type.
    virtual SDL_Texture& getTexture(Entity::Type) = 0;
};

}
