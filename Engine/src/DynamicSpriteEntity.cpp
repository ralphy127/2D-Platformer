#include "Engine/DynamicSpriteEntity.h"

namespace engine {

DynamicSpriteEntity::DynamicSpriteEntity(
    Settings& settings,
    ISpriteTextures& textures,
    const Config& config)
    : DynamicEntity(settings, config),
      _textures(textures),
      _spriteData(std::move(config.spriteData)) {}

void DynamicSpriteEntity::update(float deltaTime) {
    _spriteData.updateFrame();

    handleAttack();

    DynamicEntity::update(deltaTime);
}

void DynamicSpriteEntity::render(SDL_Renderer& renderer, Camera& camera) const {
    const auto type = getType();

    auto& texture = _textures.getTexture(type, _spriteData.getAnimation(), _spriteData.getFrame());
    
    const auto flip = getDirection() > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    
    const utils::f2v texturePos(
        getPos().x - (getTextureSize().x - getSize().x) / 2.,
        getPos().y - (getTextureSize().y - getSize().y));
    
    const auto textureRect = camera.worldToViewport(texturePos, getTextureSize());

    if (SDL_RenderCopyEx(&renderer, &texture, nullptr, &textureRect, 0, nullptr, flip) < 0)
        throw std::runtime_error(std::string("SDL_RenderCopyEx error: ") + SDL_GetError()
                                 + " while rendering entity of type: " + std::to_string(type));

    if (getSettings().showHitboxes()) {
        renderEntityHitbox(renderer, camera);
        renderWeaponHitbox(renderer, camera);
    }

    if (getSettings().showTextureHitboxes()) {
        renderTextureHitbox(renderer, camera, textureRect);
    }
}

void DynamicSpriteEntity::performAttack(AttackId id) {
    const auto it = _attacks.find(id);
    if (it == _attacks.cend())
        throw std::runtime_error("Tried to perform attack of unknown id: " + std::to_string(id));

    _currentAttack = id;
    _lastAttackStartTime = ClockType::now();

    const auto attack = it->second;
    getSpriteData().setAnimation(attack.animationId);
}

void DynamicSpriteEntity::handleAttack() {
    if (!_currentAttack.has_value())
        return;

    const auto now = ClockType::now();
    if (now - _lastAttackStartTime > _attacks[_currentAttack.value()].duration)
        _currentAttack = std::nullopt;
}

void DynamicSpriteEntity::renderWeaponHitbox(SDL_Renderer& renderer, Camera& camera) const {
    if (!_currentAttack.has_value())
        return;

    const auto it = _attacks.find(_currentAttack.value());
    if (it == _attacks.end())
        return;
    
    const auto attack = it->second;

    const auto offset = getDirection() > 0 ? attack.offsetRight : attack.offsetLeft;
    const auto size = attack.size;
    
    SDL_Rect actualWeaponHitBox = {
        static_cast<int>(getPos().x + offset.x),
        static_cast<int>(getPos().y + offset.y),
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    };
    
    const auto weaponRect = camera.worldToViewport(
        {static_cast<float>(actualWeaponHitBox.x), static_cast<float>(actualWeaponHitBox.y)},
        {static_cast<float>(actualWeaponHitBox.w), static_cast<float>(actualWeaponHitBox.h)}
    );
    
    SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255); // green
    SDL_RenderDrawRect(&renderer, &weaponRect);
}

}
