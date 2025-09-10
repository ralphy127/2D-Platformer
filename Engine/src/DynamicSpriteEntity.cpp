#include "Engine/DynamicSpriteEntity.h"

#include "Engine/Colors.h"

namespace engine {

DynamicSpriteEntity::DynamicSpriteEntity(
    Settings& settings,
    ISpriteTextures& textures,
    const Config& config)
    : DynamicEntity(settings, config),
      _textures(textures),
      _spriteData(std::move(config.spriteData)) {

    const auto size = getSize();
    const auto tileSize = getTileSize();

    _healthBarSize = {size.x * 0.9f, tileSize * 0.2f};

    updateHealthBar();
}

void DynamicSpriteEntity::update(float deltaTime) {
    _spriteData.updateFrame();

    handleAttack();

    updateHealthBar();

    calculateWeaponHitbox();

    DynamicEntity::update(deltaTime);
}

void DynamicSpriteEntity::render(SDL_Renderer& renderer, Camera& camera) const {
    const auto type = getType();
    auto& texture = _textures.getTexture(type, _spriteData.getAnimation(), _spriteData.getFrame());
    const auto flip = getDirection() == Direction::Right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    const auto pos = getPos();
    const auto size = getSize();
    const auto textureSize = getTextureSize();
    
    const utils::f2v texturePos(
        pos.x - (textureSize.x - size.x) / 2.,
        pos.y - (textureSize.y - size.y));
    
    const auto textureRect = camera.worldToViewport(texturePos, textureSize);

    const auto& settings = getSettings();

    if (settings.showHitboxes()) {
        renderEntityHitbox(renderer, camera);
        renderWeaponHitbox(renderer, camera);
    }

    if (settings.showTextureHitboxes())
        renderTextureHitbox(renderer, camera, textureRect);

    if (SDL_RenderCopyEx(&renderer, &texture, nullptr, &textureRect, 0, nullptr, flip) < 0)
        throw std::runtime_error(std::string("SDL_RenderCopyEx error: ") + SDL_GetError()
                                 + " while rendering entity of type: " + std::to_string(type));

    renderHealthBar(renderer, camera);
}

const AttackData& DynamicSpriteEntity::getCurrentAttackDataView() const {
    if (!_currentAttack.has_value())
        throw std::runtime_error("Entity is not performing an attack");
    
    return _attacks.at(_currentAttack.value());
}

void DynamicSpriteEntity::updateHealthBar() {
    const auto pos = getPos();
    const auto size = getSize();

    _healthBarPos = {pos.x + 0.5f * size.x - 0.5f * _healthBarSize.x, pos.y - getTileSize() * 0.2f};
}

void DynamicSpriteEntity::calculateWeaponHitbox() {
    if (!_currentAttack.has_value()) {
        _weaponHitbox.reset();
        return;
    }

    const auto it = _attacks.find(_currentAttack.value());
    if (it == _attacks.end())
        throw std::runtime_error(std::string("AttackData of attackId: ") +
            std::to_string(_currentAttack.value()) + "not found");
    
    const auto attack = it->second;
    const auto posOffset = getDirection() == Direction::Right ? attack.offsetRight : attack.offsetLeft;
    const auto weaponSize = attack.size;

    const auto pos = getPos();
    
    _weaponHitbox = {
        pos.x + posOffset.x,
        pos.y + posOffset.y,
        weaponSize.x,
        weaponSize.y
    };
}

void DynamicSpriteEntity::performAttack(AttackId id) {
    if (_currentAttack.has_value())
        return;

    const auto it = _attacks.find(id);
    if (it == _attacks.cend())
        throw std::runtime_error("Tried to perform attack of unknown id: " + std::to_string(id));

    _currentAttack = id;
    _lastAttackStartTime = Clock::getTime();
    _alreadyHitThisAttack.clear();

    const auto attack = it->second;
    getSpriteData().setAnimation(attack.animationId);
}

void DynamicSpriteEntity::handleAttack() {
    if (!_currentAttack.has_value())
        return;

    const auto now = Clock::getTime();
    if (now - _lastAttackStartTime > _attacks[_currentAttack.value()].duration)
        _currentAttack = std::nullopt;
}

void DynamicSpriteEntity::renderWeaponHitbox(SDL_Renderer& renderer, Camera& camera) const {
    if (!_currentAttack.has_value() || !_weaponHitbox.has_value())
        return;
    
    const auto weaponHitbox = camera.worldToViewport(
        {_weaponHitbox->x, _weaponHitbox->y},
        {_weaponHitbox->w, _weaponHitbox->h}
    );
    
    SDL_SetRenderDrawColor(
        &renderer,
        color_green.r,
        color_green.g,
        color_green.b,
        SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(&renderer, &weaponHitbox);
}

void DynamicSpriteEntity::renderHealthBar(SDL_Renderer& renderer, Camera& camera) const {
    const auto hpPercent = getHealth() / getMaxHealth();
    
    const auto backgroundRect = camera.worldToViewport(_healthBarPos, _healthBarSize);

    SDL_SetRenderDrawColor(
        &renderer,
        color_dark_grey.r,
        color_dark_grey.g,
        color_dark_grey.b,
        SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(&renderer, &backgroundRect);

    const utils::i2v healthSize = {
        static_cast<int>(hpPercent * backgroundRect.w),
        static_cast<int>(hpPercent * backgroundRect.h)
    };

    SDL_Rect barRect = backgroundRect;
    barRect.w = static_cast<int>(hpPercent * backgroundRect.w);

    if (isHealthy())
        SDL_SetRenderDrawColor(
            &renderer,
            color_green.r,
            color_green.g,
            color_green.b,
            SDL_ALPHA_OPAQUE);
    else if (isWounded())
        SDL_SetRenderDrawColor(
            &renderer,
            color_orange.r,
            color_orange.g,
            color_orange.b,
            SDL_ALPHA_OPAQUE);
    else if (isCritical())
        SDL_SetRenderDrawColor(&renderer, color_red.r, color_red.g, color_red.b, SDL_ALPHA_OPAQUE);

    SDL_RenderFillRect(&renderer, &barRect);
}

void DynamicSpriteEntity::tryToChangeAnimation(AnimationId animationId) {
    if (!isAttacking() && _spriteData.getAnimation() != animationId)
        _spriteData.setAnimation(animationId);
}

}
