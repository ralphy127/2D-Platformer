#pragma once

#include <SDL.h>
#include "Engine/Camera.h"

namespace engine {

/// @brief Interface for all renderable objects.
class IRenderable {
public:
    virtual ~IRenderable() = default;

    virtual void render(SDL_Renderer&, Camera&) const = 0;
};

}
