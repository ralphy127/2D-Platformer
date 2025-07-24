#pragma once

#include <SDL.h>
#include "Engine/Entity.h"

namespace engine {

class ISpriteTextures {
public:
    virtual ~ISpriteTextures() = default;

    virtual SDL_Texture& getTexture(Entity::Type, size_t animation, size_t frame) = 0;
};

}
