#pragma once

#include "Engine/DynamicEntity.h"
#include "Engine/ISimpleTextures.h"

namespace engine {

/// @brief Represents dynamic entity represented in game by single texture.
class DynamicSimpleEntity : public DynamicEntity {
public:
    /// @brief Configuration struct used to initialize a DynamicSimpleEntity.
    struct Config : DynamicEntity::Config {};

    DynamicSimpleEntity(Settings&, ISimpleTextures&, const Config&);

    /// @brief Updates entity's logic based on the elapsed time.
    void update(float32_t deltaTime) override {}

    void render(SDL_Renderer&, Camera&) const override;

private:
    ISimpleTextures& _textures; ///< Reference to texture provider.
};

}
