#include "Engine/Entity.h"

namespace engine {

Entity::Entity(Settings& settings, const Config& config) 
    : _settings(settings),
      _pos(config.pos),
      _size(config.size),
      _type(config.type) {

    _settings.registerObserver(*this);
    _tileSize = _settings.getTileSize();
}

void Entity::onSettingsChanged() {
    auto oldTileSize = _tileSize;

    _tileSize = _settings.getTileSize();

    _size *= (oldTileSize / _tileSize);
}

}
