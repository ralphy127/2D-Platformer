#pragma once

#include <SDL.h>
#include "Engine/EntityTypes.h"

namespace engine {

class ISpriteTextures {
public:
    virtual ~ISpriteTextures() = default;

    virtual SDL_Texture& getTexture(EntityTypes::Sprite, size_t animation, size_t frame) = 0;
};

}
