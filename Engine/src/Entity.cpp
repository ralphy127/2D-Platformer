#include "Engine/Entity.h"

namespace engine {

Entity::Entity(utils::f2v pos, utils::f2v size, Settings& settings) 
    : _settings(settings),
      _pos(pos),
      _size(size) {

    _settings.registerObserver(*this);
    _tileSize = _settings.getTileSize();
}

void Entity::onSettingsChanged() {
    auto oldTileSize = _tileSize;

    _tileSize = _settings.getTileSize();

    _size *= (oldTileSize / _tileSize);
}

}
