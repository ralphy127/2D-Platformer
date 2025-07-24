#pragma once

#include "IRenderable.h"
#include "Engine/Settings.h"
#include "Engine/Entity.h"

namespace engine {

// abstract class
class Drawable : public Entity, public IRenderable {
public:
    struct Config : Entity::Config {
        utils::f2v pos;
        utils::f2v size;
        utils::f2v textureSize;
        int direction;
    };

    Drawable(Settings&, const Config&);

    void onSettingsChanged() override;

    int getDirection() const { return _direction; }
    void setDirection(int direction) { _direction = direction; }

protected:
    utils::f2v getTextureSize() const { return _textureSize; }
    void setTextureSize(utils::f2v textureSize) { _textureSize = textureSize; }

private:
    utils::f2v _textureSize;
    int _direction;
};

}
