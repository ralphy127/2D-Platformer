#include <Engine/PhysicsHandler.h>

#include <SDL2/SDL.h>
#include <cmath>

namespace engine {

PhysicsHandler::PhysicsHandler(Settings& settings) 
    :  _settings(settings) {
    
    _settings.registerObserver(*this);

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Physics handler created");
}

PhysicsHandler::~PhysicsHandler() { 
    _settings.unregisterObserver(*this);

    SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Physics handler destroyed"); 
};

Collision PhysicsHandler::AABBcast(const SDL_Rect& source, const SDL_Rect& target, const utils::f2v& velocity, collisionHit& outhit, float deltaTime) const {
    if (std::abs(velocity.length()) <= std::numeric_limits<float>::epsilon())
        return {false, false};

    bool collisionX = false;
    bool collisionY = false;

    if (std::abs(velocity.x) > std::numeric_limits<float>::epsilon()) {
        SDL_Rect futureX = source;
        futureX.x += static_cast<int>(std::round(velocity.x * deltaTime));

        if (SDL_HasIntersection(&futureX, &target)) {
            collisionX = true;
            outhit.normal.x = (velocity.x > 0) ? -1.0f : 1.0f;
        }
    }

    if (std::abs(velocity.y) > std::numeric_limits<float>::epsilon()) {
        SDL_Rect futureY = source;
        futureY.y += static_cast<int>(velocity.y * deltaTime);

        if (SDL_HasIntersection(&futureY, &target)) {
            collisionY = true;
            outhit.normal.y = (velocity.y > 0) ? -1.0f : 1.0f;
        }
    }

    return {collisionX, collisionY};
}

void PhysicsHandler::applyGravity(DynamicEntity& entity, float deltaTime) {
    if (entity.isOnGround())
        return;

    auto vel = entity.getVel();
    const auto deltaVy = _gAcceleration * deltaTime;

    if (vel.y < 0) {
        vel.y += deltaVy;
    }
    else {
        vel.y += _fallGMultiplier * deltaVy;
    }

    entity.setVel(vel);
}

void PhysicsHandler::onSettingsChanged() {
    auto oldTileSize = _tileSize;

    _tileSize = _settings.getTileSize();

    auto tileSizeRatio = (static_cast<float>(oldTileSize) / _tileSize);

    _gAcceleration *= tileSizeRatio;
    _fallGMultiplier *= tileSizeRatio;
}

}