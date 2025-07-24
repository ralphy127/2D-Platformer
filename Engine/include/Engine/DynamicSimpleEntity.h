#pragma once

#include "Engine/DynamicEntity.h"
#include "Engine/ISimpleTextures.h"

namespace engine {

/// @brief Represents a dynamic entity using simple textures for rendering.
class DynamicSimpleEntity : public DynamicEntity {
public:
    /// @brief Configuration structure for DynamicSimpleEntity.
    /// Inherits all configuration fields from DynamicEntity::Config.
    struct Config : DynamicEntity::Config {
        // No additional fields for now.
    };

    /// @brief Constructs a DynamicSimpleEntity with texture support.
    /// @param settings Reference to engine settings.
    /// @param textures Texture provider implementing ISimpleTextures.
    /// @param config Configuration for initialization.
    DynamicSimpleEntity(Settings&, ISimpleTextures&, const Config&);

    /// @brief Updates the entity's state.
    /// @param deltaTime Time since last frame (in seconds).
    void update(float deltaTime) override {}

    /// @brief Renders the entity using the given renderer and camera.
    /// @param renderer SDL renderer to draw with.
    /// @param camera Camera used to transform world coordinates.
    void render(SDL_Renderer&, Camera&) const override;

private:
    ISimpleTextures& _textures; ///< Reference to texture provider.
};

}
