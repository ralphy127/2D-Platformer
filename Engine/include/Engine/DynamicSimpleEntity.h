#pragma once

#include "Engine/DynamicEntity.h"
#include "Engine/EntityTypes.h"
#include "Engine/ISimpleTextures.h"

namespace engine {

class DynamicSimpleEntity : public DynamicEntity {
public:
    struct Config : DynamicEntity::Config {
        EntityTypes::Simple type;
    };

    DynamicSimpleEntity(Settings&, ISimpleTextures&, const Config&);

    void update(float deltaTime) override {} // for now empty

    void render(SDL_Renderer&, Camera&) const override;

private:
    ISimpleTextures& _textures;
    EntityTypes::Simple _type;


};

}
