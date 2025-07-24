#include "Engine/DynamicEntity.h"

namespace engine {

DynamicEntity::DynamicEntity(Settings& settings, const Config& config) 
    : Drawable(settings, config),
      _maxHealth(config.maxHealth),
      _defaultSpeed(config.defaultSpeed),
      _sprintSpeed(config.sprintSpeed) {

    capHealthIfNeeded(_maxHealth);
}

void DynamicEntity::update(float deltaTime) {
    auto vel = getVel();
    setPos(getPos() + utils::f2v(vel.x * deltaTime, vel.y * deltaTime));
}

}
