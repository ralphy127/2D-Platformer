#pragma once

#include <SDL.h>
#include "Engine/EntityTypes.h"

namespace engine {

class ISimpleTextures {
public:
    virtual ~ISimpleTextures() = default;

    virtual SDL_Texture& getTexture(EntityTypes::Simple) = 0;
};

}
