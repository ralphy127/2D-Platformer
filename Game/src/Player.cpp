#include "Game/Player.h"

#include <chrono>

namespace game {

Player::Player(engine::Settings& settings, engine::ISpriteTextures& textures)
    : DynamicSpriteEntity(settings, textures, initAndGetConfig(settings)) {
    
    auto tileSize = getTileSize();
    setTextureSize({3.f * tileSize, 3.f * tileSize});
    setSize({0.85f * tileSize, 1.8f * tileSize});
    setPos({20.f * tileSize, 30.f * tileSize - getSize().y});
}

void Player::update(float deltaTime, const std::unordered_map<SDL_Keycode, bool>& keyStates) {
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
    config.pos = {0.3f * windowSize.x, 30.f * tileSize - config.size.y};
    config.textureSize = {3.f * tileSize, 3.f * tileSize};
    config.direction = 1;
    config.maxHealth = 100.f;
    config.health = 100.f;
    config.spriteData = std::move(spriteData);
    config.type = engine::EntityTypes::Sprite::PLAYER;

    return config;
}

}
