#include <Game/Commander.h>

#include <Game/EntityTypes.h>

namespace game {

Commander::Commander(engine::Settings& settings, engine::ISpriteTextures& textures)
    : DynamicSpriteEntity(settings, textures, initAndGetConfig(settings)) {}

void Commander::update(float deltaTime) {
    auto& spriteData = getSpriteData();

    if (_state == State::Dead) {
        tryToChangeAnimation(static_cast<engine::AnimationId>(_state));

        if (spriteData.getFrame() == spriteData.getFramesInCurrentAnimation() - 1 &&
            !spriteData.isFrameLocked())
            spriteData.lockFrame();
    }
    else {
        const auto now = engine::Clock::getTime();

        if (now - _lastBehaviorChange > _interval) {
            if (_state == State::Idle) {
                turn();
                move();
                _state = State::Walking;
            }
            else if (_state == State::Walking) {
                stop();
                _state = State::Idle;
            }

            _lastBehaviorChange = now;
            spriteData.setAnimation(static_cast<engine::AnimationId>(_state));
        }
    }

    DynamicSpriteEntity::update(deltaTime);
}

void Commander::handleDeath() {
    setVel({0.f, getVel().y});
    _state = State::Dead;
}

engine::DynamicSpriteEntity::Config Commander::initAndGetConfig(
    const engine::Settings& settings) const {
        
    const auto framesDuration = std::chrono::milliseconds(120);

    // number of frames / frame duration
    engine::SpriteData::AnimationsInfo animationsInfo = {
        {5, framesDuration},
        {9, framesDuration},
        {8, framesDuration},
        {4, framesDuration},
        {5, framesDuration},
        {4, framesDuration},
        {2, framesDuration},
        {7, framesDuration},
        {2, framesDuration},
        {6, framesDuration}
    };

    engine::SpriteData spriteData{std::move(animationsInfo), static_cast<engine::AnimationId>(_state)};

    const auto windowSize = settings.getWindowSize();
    const auto tileSize = settings.getTileSize();

    engine::DynamicSpriteEntity::Config config{};

    config.size = {1.33f * 0.85f * tileSize, 1.33f * 1.8f * tileSize};
    config.pos = {0.3f * windowSize.x, -30.f * tileSize - config.size.y};
    config.type = static_cast<engine::Entity::Type>(EntityType::Commander);
    config.textureSize = {1.33f * 3.f * tileSize, 1.33f * 3.f * tileSize};
    config.direction = engine::Direction::Right;
    config.maxHealth = 100.f;
    config.health = 100.f;
    config.defaultSpeed = 0.9f * tileSize;
    config.sprintSpeed = 5.5f * tileSize;
    config.defaultJumpVy = 5.f * tileSize;
    config.spriteData = std::move(spriteData);

    return config;
}

}