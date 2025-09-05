#include "Engine/Drawable.h"

namespace engine {

Drawable::Drawable(Settings& settings, const Config& config)
    : Entity(settings, config),
      _textureSize(config.textureSize),
      _direction(config.direction) {}

int8_t Drawable::getDirectionMultiplicator() const {
    if (_direction == Direction::Left)
        return -1;
    if (_direction == Direction::Right)
        return 1;
    return 0;
}

void Drawable::turn() {
    if (_direction == Direction::None)
        return;
    _direction = _direction == Direction::Left ? Direction::Right : Direction::Left;
}

void Drawable::onSettingsChanged() {
    auto oldTileSize = getTileSize();

    Entity::onSettingsChanged();

    auto newTileSize = getTileSize();

    _textureSize *= (static_cast<float>(oldTileSize) / newTileSize);
}

}
