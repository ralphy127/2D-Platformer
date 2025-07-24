#pragma once

#include "Engine/DynamicEntity.h"
#include "Engine/ISpriteTextures.h"
#include "Engine/SpriteData.h"

namespace engine {

class DynamicSpriteEntity : public DynamicEntity {
public:
    struct Config : DynamicEntity::Config {
        SpriteData spriteData;
    };

    DynamicSpriteEntity(Settings&, ISpriteTextures&, const Config&);

    void update(float deltaTime) override;
    void render(SDL_Renderer&, Camera&) const override;

protected:
    SpriteData& getSpriteData() { return _spriteData; }

private:
    ISpriteTextures& _textures;
    SpriteData _spriteData;
};

}
