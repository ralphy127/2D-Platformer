#pragma once

#include <SDL.h>

namespace engine {

class IMapTextures {
public:
    virtual ~IMapTextures() = default;

    virtual SDL_Texture& getTexture(int type, size_t id) = 0;
};

}