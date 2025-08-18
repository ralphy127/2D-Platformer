#pragma once

#include <Utils/SDLUtils.h>
#include "Engine/Layer.h"
#include "Engine/TexturesManager.h"
#include "Camera.h"

namespace engine {

class ParallaxLayer : public ILayer, public ISettingsObserver {
public:
    enum class Type { BACKGROUND, FAR, NEAR };

    ParallaxLayer(Settings&, SDL_Renderer&, Type, size_t levelId, float parallaxSpeed);

    ~ParallaxLayer() { _settings.unregisterObserver(*this); }

    void render(SDL_Renderer&, Camera&) const override;

    void onSettingsChanged() override;

private:
    std::string getTexturePath() const;

    Settings& _settings;

    Type _type;
    size_t _levelId;
    float _parallaxSpeed;
    size_t _tileSize;
    utils::SDLUtils::TexturePtr _texture{nullptr, SDL_DestroyTexture};
};

}