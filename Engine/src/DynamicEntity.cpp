#include "Engine/DynamicEntity.h"

#include "Engine/Colors.h"

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

void DynamicEntity::capHealthIfNeeded(float health) {
    if (health > _maxHealth) {
        _health = _maxHealth;
    }
    else if (health < 0) {
        _health = 0.f;
    }
    else {
        _health = health;
    }
}

void DynamicEntity::renderEntityHitbox(SDL_Renderer& renderer, Camera& camera) const {
    auto entityRect = camera.worldToViewport(getPos(), getSize());
    SDL_SetRenderDrawColor(&renderer, color_red.r, color_red.g, color_red.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(&renderer, &entityRect);
}

void DynamicEntity::renderTextureHitbox(
    SDL_Renderer& renderer,
    Camera& camera,
    const SDL_Rect& textureRect) const {
        
    SDL_SetRenderDrawColor(&renderer, color_grey.r, color_grey.g, color_grey.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(&renderer, &textureRect);
}

}
