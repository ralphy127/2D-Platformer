#include "Game/Player.h"

#include <chrono>
#include <sstream>
#include <iomanip>
#include <Game/EntityTypes.h>

namespace game {

Player::Player(
    engine::Settings& settings,
    engine::ISpriteTextures& textures,
    const engine::EventHandler& eventHandler)
    : DynamicSpriteEntity(settings, textures, initAndGetConfig(settings)),
      _eventHandler(eventHandler) {

    createAndAddAttacks();
}

void Player::update(float deltaTime) {
    handleMovement(deltaTime);
    handleAttacks();
    
    DynamicSpriteEntity::update(deltaTime);
}

void Player::render(SDL_Renderer& renderer, engine::Camera& camera) const {
    auto pos = getPos();
    auto size = getSize();
    utils::f2v cameraPos(pos.x - size.x/2.f,
        pos.y - size.y/2.f -  getSettings().getWindowSize().y * 0.18f / camera.getZoom());
    camera.centerOn(cameraPos);

    DynamicSpriteEntity::render(renderer, camera);
}

void Player::logDebugState() {
    constexpr auto logInterval = std::chrono::seconds(1);
    const std::string logKey{"PlayerState"};

    if (!utils::Logger::shouldLog(logKey, logInterval))
        return;

    constexpr int whitespaces = 45;
    constexpr int labelWidth = 15;

    std::ostringstream message;
    message << "\n";

    auto addFieldToMessage = [&message](const std::string& label, const auto value) {
        message << std::right << std::setw(whitespaces) << ' ' 
                << std::left << std::setw(labelWidth) << ("[" + label + "]")
                << " : " << value << "\n";
    };

    const auto& spriteData = getSpriteData();

    addFieldToMessage("Position", getPos());
    addFieldToMessage("Velocity", getVel());
    addFieldToMessage("On Ground", isOnGround() ? "True" : "False");
    addFieldToMessage("Direction", getDirection() == engine::Direction::Right ? "Right" : "Left");
    addFieldToMessage("Animation", stateToString(static_cast<State>(spriteData.getAnimation())));
    addFieldToMessage("Frame", spriteData.getFrame());

    utils::Logger::logDebugEvery(logKey, logInterval, message.str());
}

engine::DynamicSpriteEntity::Config Player::initAndGetConfig(
    const engine::Settings& settings) const {

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

    engine::SpriteData spriteData{std::move(animationsInfo), static_cast<engine::AnimationId>(_state)};

    const auto windowSize = settings.getWindowSize();
    const auto tileSize = settings.getTileSize();

    engine::DynamicSpriteEntity::Config config{};

    config.size = {0.85f * tileSize, 1.8f * tileSize};
    config.pos = {0.3f * windowSize.x, -30.f * tileSize - config.size.y};
    config.type = static_cast<engine::Entity::Type>(EntityType::Player);
    config.textureSize = {3.f * tileSize, 3.f * tileSize};
    config.direction = engine::Direction::Right;
    config.maxHealth = 100.f;
    config.health = 100.f;
    config.defaultSpeed = 4.5f * tileSize;
    config.sprintSpeed = 7.5f * tileSize;
    config.defaultJumpVy = 5.f * tileSize;
    config.spriteData = std::move(spriteData);

    return config;
}

void Player::createAndAddAttacks() {
    const auto size = getSize();

    engine::AttackData attack;
    attack.id = static_cast<engine::AttackId>(State::Attack1);
    attack.damage = 50.f;
    attack.duration = 4 * std::chrono::milliseconds(120);
    attack.offsetRight = {0.6f * size.x, 0.2f * size.y};
    attack.offsetLeft = {-1.15f * size.x, 0.2f * size.y};
    attack.size = {1.75f * size.x, 0.45f * size.y};
    attack.animationId = static_cast<engine::AnimationId>(State::Attack1);
    attack.onHit = [](){
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player hit an entity with attack 1"); };
    attack.sourceTag = "player";

    addAtack(attack);

    attack.id = static_cast<engine::AttackId>(State::Attack2);
    attack.damage = 70.f;
    attack.duration = 5 * std::chrono::milliseconds(120);
    attack.offsetRight = {0.15f * size.x, -0.45f * size.y};
    attack.offsetLeft = {-1.3f * size.x, -0.45f * size.y};
    attack.size = {2.3f * size.x, 1.3f * size.y};
    attack.animationId = static_cast<engine::AnimationId>(State::Attack2);
    attack.onHit = [](){
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player hit an entity with attack 2"); };

    addAtack(attack);

    attack.id = static_cast<engine::AttackId>(State::Attack3);
    attack.damage = 35.f;
    attack.duration = 4 * std::chrono::milliseconds(120);
    attack.offsetRight = {-0.1f * size.x, 0.05f * size.y};
    attack.offsetLeft = {-0.95f * size.x, 0.05f * size.y};
    attack.size = {2.1f * size.x, 0.65f * size.y};
    attack.animationId = static_cast<engine::AnimationId>(State::Attack3);
    attack.onHit = [](){
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player hit an entity with attack 3"); };

    addAtack(std::move(attack));
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
        setDirection(engine::Direction::Left);
        handleLShift(lshiftPressed, spriteData);
    }
    else if (rightPressed) {
        setDirection(engine::Direction::Right);
        handleLShift(lshiftPressed, spriteData);
    }
    else {
        stop();
        tryToChangeAnimation(State::Idle, spriteData);
    }

    if (!isOnGround()) {
        tryToChangeAnimation(State::Jumping, spriteData);
    }

    if(jumpNow && !_jumpPressedLastFrame && isOnGround()) {
        jump();
        tryToChangeAnimation(State::Jumping, spriteData);
    }

    _jumpPressedLastFrame = jumpNow;
}

void Player::handleLShift(bool shiftPressed, engine::SpriteData& spriteData) {
    if (shiftPressed) {
        sprint();
        tryToChangeAnimation(State::Running, spriteData);
    } 
    else {
        move();
        tryToChangeAnimation(State::Walking, spriteData);
    }
}

void Player::tryToChangeAnimation(State state, engine::SpriteData& spriteData) {
    if (!isAttacking() && spriteData.getAnimation() != static_cast<engine::AnimationId>(state))
        spriteData.setAnimation(static_cast<engine::AnimationId>(state));
}

void Player::handleAttacks() {
    const auto& spriteData = getSpriteData();
    if (_eventHandler.isKeyPressed(SDLK_z) &&
        spriteData.getAnimation() != static_cast<engine::AnimationId>(State::Attack1)) {
            
        performAttack(static_cast<engine::AttackId>(State::Attack1));
    }
    else if (_eventHandler.isKeyPressed(SDLK_x) &&
             spriteData.getAnimation() != static_cast<engine::AnimationId>(State::Attack2)) {
        performAttack(static_cast<engine::AttackId>(State::Attack2));
    }
    else if (_eventHandler.isKeyPressed(SDLK_c) &&
             spriteData.getAnimation() != static_cast<engine::AnimationId>(State::Attack3)) {
        performAttack(static_cast<engine::AttackId>(State::Attack3));
    }
}

std::string Player::stateToString(State state) {
    switch (state) {
        case State::Idle:       return "Idle";
        case State::Walking:    return "Walking";
        case State::Running:    return "Running";
        case State::Jumping:    return "Jumping";
        case State::Attack1:    return "Attack1";
        case State::Attack2:    return "Attack2";
        case State::Attack3:    return "Attack3";
        case State::Protecting: return "Protecting";
        case State::Hurt:       return "HURT";
        case State::Dead:       return "Dead";
        default:                return "UNKNOWN";
    }
}

}
