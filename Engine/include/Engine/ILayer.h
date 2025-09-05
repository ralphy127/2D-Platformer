#pragma once

#include <SDL.h>
#include "Engine/Camera.h"

namespace engine {

using LevelId = std::uint16_t;

/// @brief Interface for renderable map layers.
class ILayer {
public:
    virtual ~ILayer() = default;

    virtual void render(SDL_Renderer&, Camera&) const = 0;
};

}
