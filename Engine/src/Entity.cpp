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

SDL_Rect Entity::getHitBox() const {
    return {
        static_cast<int>(_pos.x),
        static_cast<int>(_pos.y),
        static_cast<int>(_size.x),
        static_cast<int>(_size.y)
    };
}

void Entity::setHitBox(SDL_Rect hitbox) {
    _pos = utils::f2v(hitbox.x, hitbox.y);
    _size = utils::f2v(hitbox.w, hitbox.h);
}

void Entity::onSettingsChanged() {
    auto oldTileSize = _tileSize;

    _tileSize = _settings.getTileSize();

    _size *= (oldTileSize / _tileSize);
}

}