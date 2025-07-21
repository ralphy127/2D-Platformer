#pragma once

#include "Engine/DynamicEntity.h"
#include "Engine/EntityTypes.h"
#include "Engine/ISpriteTextures.h"
#include "Engine/SpriteData.h"

namespace engine {

class DynamicSpriteEntity : public DynamicEntity {
public:
    struct Config : DynamicEntity::Config {
        SpriteData spriteData;
        EntityTypes::Sprite type;
    };

    DynamicSpriteEntity(Settings&, ISpriteTextures&, const Config&);

    void update(float deltaTime) override { _spriteData.updateFrame(); }
    void render(SDL_Renderer&, Camera&) const override;

private:
    ISpriteTextures& _textures;
    SpriteData _spriteData;
    EntityTypes::Sprite _type;
};

}
