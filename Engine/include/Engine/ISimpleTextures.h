#pragma once

#include <SDL.h>
#include "Engine/Entity.h"

namespace engine {

class ISimpleTextures {
public:
    virtual ~ISimpleTextures() = default;

    virtual SDL_Texture& getTexture(Entity::Type) = 0;
};

}
