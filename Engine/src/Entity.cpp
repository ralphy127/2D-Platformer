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

SDL_FRect Entity::getHitbox() const {
    return {
        _pos.x,
        _pos.y,
        _size.x,
        _size.y
    };
}

void Entity::setHitbox(SDL_Rect hitbox) {
    _pos = utils::f2v(hitbox.x, hitbox.y);
    _size = utils::f2v(hitbox.w, hitbox.h);
}

void Entity::onSettingsChanged() {
    auto oldTileSize = _tileSize;

    _tileSize = _settings.getTileSize();

    _size *= (oldTileSize / _tileSize);
}

}