#include "Game/Player.h"

#include <chrono>
#include <Game/EntityTypes.h>

namespace game {

Player::Player(engine::Settings& settings, engine::ISpriteTextures& textures, const engine::EventHandler& eventHandler)
    : DynamicSpriteEntity(settings, textures, initAndGetConfig(settings)),
      _eventHandler(eventHandler) {}

void Player::update(float deltaTime) {
    handleMovement(deltaTime);
    
    DynamicSpriteEntity::update(deltaTime);
}

void Player::render(SDL_Renderer& renderer, engine::Camera& camera) const {
    auto pos = getPos();
    auto size = getSize();
    utils::f2v cameraPos(pos.x - size.x/2.f, pos.y - size.y/2.f);
    camera.centerOn(cameraPos);
    
    DynamicSpriteEntity::render(renderer, camera);
}

engine::DynamicSpriteEntity::Config Player::initAndGetConfig(engine::Settings& settings) const {
    const auto framesDuration = std::chrono::milliseconds(120);

    engine::SpriteData::AnimationsInfo animationsInfo = {
        {6, framesDuration},
        {9, framesDuration},
        {8, framesDuration},
        {4, framesDuration},
        {5, framesDuration},
        {4, framesDuration},
        {2, framesDuration},
        {9, framesDuration},
        {3, framesDuration},
        {6, framesDuration}
    };

    engine::SpriteData spriteData{std::move(animationsInfo), static_cast<size_t>(_state)};

    const auto windowSize = settings.getWindowSize();
    const auto tileSize = settings.getTileSize();

    engine::DynamicSpriteEntity::Config config{};

    config.size = {0.85f * tileSize, 1.8f * tileSize};
    config.pos = {0.3f * windowSize.x, -30.f * tileSize - config.size.y};
    config.type = static_cast<engine::Entity::Type>(EntityType::PLAYER);
    config.textureSize = {3.f * tileSize, 3.f * tileSize};
    config.direction = 1;
    config.maxHealth = 100.f;
    config.health = 100.f;
    config.defaultSpeed = 2.5f * tileSize;
    config.sprintSpeed = 7.5f * tileSize;
    config.defaultJumpVy = 5.f * tileSize;
    config.spriteData = std::move(spriteData);

    return config;
}

void Player::handleMovement(float deltaTime) {
    const auto leftPressed = _eventHandler.isAnyKeyPressed(SDLK_LEFT, SDLK_a);
    const auto rightPressed = _eventHandler.isAnyKeyPressed(SDLK_RIGHT, SDLK_d);
    const bool lshiftPressed = _eventHandler.isKeyPressed(SDLK_LSHIFT);

    const bool jumpNow = _eventHandler.isAnyKeyPressed(SDLK_UP, SDLK_w);

    auto& spriteData = getSpriteData();

    if (leftPressed && rightPressed) {
        handleLShift(lshiftPressed, spriteData);
    }
    else if (leftPressed) {
        setDirection(-1);
        handleLShift(lshiftPressed, spriteData);
    }
    else if (rightPressed) {
        setDirection(1);
        handleLShift(lshiftPressed, spriteData);
    }
    else {
        stop();
        if (spriteData.getAnimation() != static_cast<size_t>(State::IDLE)) {
            spriteData.setAnimation(static_cast<size_t>(State::IDLE));
        }
    }

    if (!isOnGround()) {
        if (spriteData.getAnimation() != static_cast<size_t>(State::JUMPING)) {
            spriteData.setAnimation(static_cast<size_t>(State::JUMPING));
        }
    }

    if(jumpNow && !_jumpPressedLastFrame && isOnGround()) {
        jump();
        if (spriteData.getAnimation() != static_cast<size_t>(State::JUMPING)) {
            spriteData.setAnimation(static_cast<size_t>(State::JUMPING));
        }
    }

    _jumpPressedLastFrame = jumpNow;
}

void Player::handleLShift(bool shiftPressed, engine::SpriteData& spriteData) {
    if (shiftPressed) {
        sprint();
        if (spriteData.getAnimation() != static_cast<size_t>(State::RUNNING)) {
            spriteData.setAnimation(static_cast<size_t>(State::RUNNING));
        }
    } 
    else {
        move();
        if (spriteData.getAnimation() != static_cast<size_t>(State::WALKING)) {
            spriteData.setAnimation(static_cast<size_t>(State::WALKING));
        }
    }
}

}
