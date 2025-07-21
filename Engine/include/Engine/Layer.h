#pragma once

#include <SDL.h>
#include "Engine/Camera.h"

namespace engine {

/// @brief Interface for renderable map layers.
class Layer {
public:
    /// @brief Virtual destructor.
    virtual ~Layer() = default;

    /// @brief Renders the layer.
    /// @param renderer SDL renderer used for drawing.
    /// @param camera Camera for applying transformations.
    virtual void render(SDL_Renderer&, Camera&) const = 0;
};

}
