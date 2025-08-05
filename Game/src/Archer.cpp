#include <Game/Archer.h>

#include <Game/EntityTypes.h>

namespace game {

Archer::Archer(engine::Settings& settings, engine::ISpriteTextures& textures)
    : DynamicSpriteEntity(settings, textures, initAndGetConfig(settings)) {}

void Archer::update(float deltaTime) {
    const auto now = engine::Clock::getTime();

    if (now - _lastBehaviorChange > _interval) {
        if (_state == State::IDLE) {
            sprint();
            _state = State::RUNNING;
        }
        else if (_state == State::RUNNING) {
            stop();
            turn();
            _state = State::IDLE;
        }

        _lastBehaviorChange = now;
        getSpriteData().setAnimation(static_cast<size_t>(_state));
    }

    DynamicSpriteEntity::update(deltaTime);
}

engine::DynamicSpriteEntity::Config Archer::initAndGetConfig(
    const engine::Settings& settings) const {
        
    const auto framesDuration = std::chrono::milliseconds(120);

    // number of frames / frame duration
    engine::SpriteData::AnimationsInfo animationsInfo = {
        {9, framesDuration},
        {8, framesDuration},
        {8, framesDuration},
        {5, framesDuration},
        {5, framesDuration},
        {6, framesDuration},
        {14, framesDuration},
        {9, framesDuration},
        {3, framesDuration},
        {5, framesDuration}
    };

    engine::SpriteData spriteData{std::move(animationsInfo), static_cast<size_t>(_state)};

    const auto windowSize = settings.getWindowSize();
    const auto tileSize = settings.getTileSize();

    engine::DynamicSpriteEntity::Config config{};

    config.size = {1.1f * 0.85f * tileSize, 1.1f * 1.8f * tileSize};
    config.pos = {0.4f * windowSize.x, -30.f * tileSize - config.size.y};
    config.type = static_cast<engine::Entity::Type>(EntityType::ARCHER);
    config.textureSize = {1.1f * 3.f * tileSize, 1.1f * 3.f * tileSize};
    config.direction = 1;
    config.maxHealth = 60.f;
    config.health = 60.f;
    config.defaultSpeed = 2.f * tileSize;
    config.sprintSpeed = 4.f * tileSize;
    config.defaultJumpVy = 5.f * tileSize;
    config.spriteData = std::move(spriteData);

    return config;
}

}