#include "Engine/DynamicEntity.h"

namespace engine {

DynamicEntity::DynamicEntity(Settings& settings, const Config& config) 
    : Drawable(settings, config),
      _maxHealth(config.maxHealth),
      _defaultSpeed(config.defaultSpeed),
      _sprintSpeed(config.sprintSpeed),
      _defaultJumpVy(config.defaultJumpVy) {

    capHealthIfNeeded(_maxHealth);
}

void DynamicEntity::update(float deltaTime) {
    ;
}

void DynamicEntity::applyMovement(float deltaTime) {
    setPos(getPos() + utils::f2v(_vel.x * deltaTime, _vel.y * deltaTime));
}

void DynamicEntity::jump() {
    if (_onGround) {
        _vel.y = -_defaultJumpVy;
        _onGround = false;
    }
}

}
