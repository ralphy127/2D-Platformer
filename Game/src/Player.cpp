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
    utils::f2v cameraPos(pos.x - size.x/2.f, pos.y - size.y/2.f);
    camera.centerOn(cameraPos);

    DynamicSpriteEntity::render(renderer, camera);
}

void Player::logDebugState() {
    const auto logInterval = std::chrono::seconds(1);
    const std::string logKey{"PlayerState"};
    if (!utils::Logger::shouldLog(logKey, logInterval))
        return;

    utils::Logger::logDebugEvery(logKey, logInterval, [&] {
        std::ostringstream oss;
        
        const int logWidth = 61;

        oss << ":\n"
            << std::right << std::setw(logWidth) << "[Position]    : " << getPos() << "\n"
            << std::right << std::setw(logWidth) << "[Velocity]    : " << getVel() << "\n"
            << std::right << std::setw(logWidth) << "[On Ground]   : " << std::boolalpha << isOnGround() << "\n"
            << std::right << std::setw(logWidth) << "[Direction]   : " << (getDirection() == 1 ? "Right" : "Left") << "\n"
            << std::right << std::setw(logWidth) << "[Animation]   : " << stateToString(getSpriteData().getAnimation()) << "\n"
            << std::right << std::setw(logWidth) << "[Frame]       : " << getSpriteData().getFrame() << "\n";
        
        return oss.str();
    });
}

engine::DynamicSpriteEntity::Config Player::initAndGetConfig(const engine::Settings& settings) const {
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
    config.defaultSpeed = 4.5f * tileSize;
    config.sprintSpeed = 7.5f * tileSize;
    config.defaultJumpVy = 5.f * tileSize;
    config.spriteData = std::move(spriteData);

    return config;
}

void Player::createAndAddAttacks() {
    const auto size = getSize();

    engine::AttackData attack;
    attack.id = static_cast<engine::AttackId>(State::ATTACK1);
    attack.damage = 50.f;
    attack.duration = 4 * std::chrono::milliseconds(120);
    attack.offsetRight = {0.6f * size.x, 0.2f * size.y};
    attack.offsetLeft = {-1.15f * size.x, 0.2f * size.y};
    attack.size = {1.75f * size.x, 0.45f * size.y};
    attack.animationId = static_cast<size_t>(State::ATTACK1);
    attack.onHit = [](){ SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player swung performed attack 1"); };
    attack.sourceTag = "player";

    addAtack(attack);

    attack.id = static_cast<engine::AttackId>(State::ATTACK2);
    attack.damage = 70.f;
    attack.duration = 5 * std::chrono::milliseconds(120);
    attack.offsetRight = {0.15f * size.x, -0.45f * size.y};
    attack.offsetLeft = {-2.15f * size.x, -0.45f * size.y};
    attack.size = {2.3f * size.x, 1.3f * size.y};
    attack.animationId = static_cast<size_t>(State::ATTACK2);
    attack.onHit = [](){ SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player swung performed attack 2"); };

    addAtack(attack);

    attack.id = static_cast<engine::AttackId>(State::ATTACK3);
    attack.damage = 35.f;
    attack.duration = 4 * std::chrono::milliseconds(120);
    attack.offsetRight = {-0.1f * size.x, 0.05f * size.y};
    attack.offsetLeft = {-2.15f * size.x, 0.05f * size.y};
    attack.size = {2.1f * size.x, 0.65f * size.y};
    attack.animationId = static_cast<size_t>(State::ATTACK3);
    attack.onHit = [](){ SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player swung performed attack 3"); };

    addAtack(attack);
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
        tryToChangeAnimation(State::IDLE, spriteData);
    }

    if (!isOnGround()) {
        tryToChangeAnimation(State::JUMPING, spriteData);
    }

    if(jumpNow && !_jumpPressedLastFrame && isOnGround()) {
        jump();
        tryToChangeAnimation(State::JUMPING, spriteData);
    }

    _jumpPressedLastFrame = jumpNow;
}

void Player::handleLShift(bool shiftPressed, engine::SpriteData& spriteData) {
    if (shiftPressed) {
        sprint();
        tryToChangeAnimation(State::RUNNING, spriteData);
    } 
    else {
        move();
        tryToChangeAnimation(State::WALKING, spriteData);
    }
}

void Player::tryToChangeAnimation(State state, engine::SpriteData& spriteData) {
    if (!isAttacking() &&
        spriteData.getAnimation() != static_cast<size_t>(state)) {

        spriteData.setAnimation(static_cast<size_t>(state));
    }
}

void Player::handleAttacks() {
    if (_eventHandler.isKeyPressed(SDLK_z)) {
        performAttack(static_cast<engine::AttackId>(State::ATTACK1));
    }
    else if (_eventHandler.isKeyPressed(SDLK_x)) {
        performAttack(static_cast<engine::AttackId>(State::ATTACK2));
    }
    else if (_eventHandler.isKeyPressed(SDLK_c)) {
        performAttack(static_cast<engine::AttackId>(State::ATTACK3));
    }
}

std::string Player::stateToString(size_t animation) {
    switch (static_cast<State>(animation)) {
        case State::IDLE:       return "IDLE";
        case State::WALKING:    return "WALKING";
        case State::RUNNING:    return "RUNNING";
        case State::JUMPING:    return "JUMPING";
        case State::ATTACK1:    return "ATTACK1";
        case State::ATTACK2:    return "ATTACK2";
        case State::ATTACK3:    return "ATTACK3";
        case State::PROTECTING: return "PROTECTING";
        case State::DEAD:       return "DEAD";
        case State::HURT:       return "HURT";
        default:                return "UNKNOWN";
    }
}

}
