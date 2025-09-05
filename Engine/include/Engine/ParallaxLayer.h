#pragma once

#include <Utils/SDLUtils.h>
#include "Engine/ILayer.h"
#include "Engine/TexturesManager.h"
#include "Camera.h"

namespace engine {

/// @brief Layer that scrolls at different speeds to create depth illusion.
class ParallaxLayer : public ILayer, public ISettingsObserver {
public:
    /// @brief Types of parallax layers with different depth levels.
    enum class Type { BACKGROUND, FAR, NEAR };

    ParallaxLayer(Settings&, SDL_Renderer&, Type, LevelId, float32_t parallaxSpeed);
    ~ParallaxLayer() { _settings.unregisterObserver(*this); }

    /// @brief Renders the parallax layer with camera-based scrolling.
    void render(SDL_Renderer&, Camera&) const override;

    void onSettingsChanged() override;

private:
    /// @brief Gets the texture file path based on type and level.
    std::string getTexturePath() const;

    Settings& _settings;  ///< Reference to game settings.

    Type _type;               ///< Layer depth type.
    LevelId _levelId;         ///< Level identifier for texture.
    float32_t _parallaxSpeed; ///< Speed multiplier for scrolling.
    uint32_t _tileSize;       ///< Size of tiles for scaling.
    utils::SDLUtils::TexturePtr _texture{nullptr, SDL_DestroyTexture}; ///< Layer texture.
};

}