#include "Engine/Drawable.h"

namespace engine {

Drawable::Drawable(Settings& settings, const Config& config)
    : Entity(settings, config),
      _textureSize(config.textureSize),
      _direction(config.direction) {}

void Drawable::onSettingsChanged() {
    auto oldTileSize = getTileSize();

    Entity::onSettingsChanged();

    auto newTileSize = getTileSize();

    _textureSize *= (oldTileSize / newTileSize);
}

}
