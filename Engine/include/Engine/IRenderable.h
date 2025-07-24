#pragma once

#include <SDL.h>
#include "Engine/Camera.h"

namespace engine {

/// @brief Interface for all renderable objects.
class IRenderable {
public:
    virtual ~IRenderable() = default;

    /// @brief Renders the object on the screen.
    /// @param renderer SDL renderer used for drawing.
    /// @param camera Camera used to adjust object position and size.
    virtual void render(SDL_Renderer&, Camera&) const = 0;
};

}
