#pragma once

#include <SDL.h>
#include "Engine/Entity.h"

namespace engine {

/// @brief Interface for animated textures provider (for sprite-type entities).
class ISpriteTextures {
public:
    virtual ~ISpriteTextures() = default;

    /// @brief Retrieves the SDL_Texture for a specific entity type, animation, and frame.
    /// @param type The type of the entity.
    /// @param animation Index of the animation.
    /// @param frame Index of the frame within the animation.
    /// @return Reference to the SDL_Texture for the specified animation frame.
    virtual SDL_Texture& getTexture(Entity::Type, size_t animation, size_t frame) = 0;
};

}
