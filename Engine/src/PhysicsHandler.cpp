#include <Engine/PhysicsHandler.h>

#include <SDL2/SDL.h>
#include <cmath>
#include <Engine/DynamicEntity.h>

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

bool PhysicsHandler::AABBcast(
    const SDL_Rect& source,
    const SDL_Rect& target,
    const utils::f2v& vel,
    collisionHit& outhit,
    float deltaTime) const {

    if (std::abs(vel.length()) <= std::numeric_limits<float>::epsilon())
        return false;

    bool collisionX = false;
    bool collisionY = false;

    if (std::abs(vel.x) > std::numeric_limits<float>::epsilon()) {
        SDL_Rect futureX = source;
        futureX.x += static_cast<int>(std::round(vel.x * deltaTime));

        if (SDL_HasIntersection(&futureX, &target)) {
            collisionX = true;
            outhit.normal.x = (vel.x > 0.f) ? -1.0f : 1.0f;
        }
    }

    if (std::abs(vel.y) > std::numeric_limits<float>::epsilon()) {
        SDL_Rect futureY = source;
        futureY.y += static_cast<int>(std::round(vel.y * deltaTime));

        if (SDL_HasIntersection(&futureY, &target)) {
            collisionY = true;
            outhit.normal.y = (vel.y > 0) ? -1.0f : 1.0f;
        }
    }

    return collisionX || collisionY;
}

void PhysicsHandler::applyGravity(DynamicEntity& entity, float deltaTime) const {
    auto vel = entity.getVel();
    const auto deltaVy = _gAcceleration * deltaTime;

    if (!entity.isOnGround()) {
        if (vel.y < 0) {
            vel.y += deltaVy;
        }
        else {
            vel.y += _fallGMultiplier * deltaVy;
        }
    }
    else {
        vel.y = 0.f;
    }

    entity.setVel(vel);
}

void PhysicsHandler::handleMapCollisions(
    DynamicEntity& entity,
    const TileLayer::Grid& map,
    float deltaTime) const {

    auto velocity = entity.getVel();
    auto hitBox = entity.getHitBox();
    
    const bool wasOnGround = entity.isOnGround();
    bool landed = false;

    const int mapHeight = static_cast<int>(map.size());

    // Check horizontal collision
    if (velocity.x != 0.f) {
        SDL_Rect futureHitBox = hitBox;
        futureHitBox.x += static_cast<int>(std::round(velocity.x * deltaTime));

        const int topTileY = futureHitBox.y / _tileSize;
        const int bottomTileY = (futureHitBox.y + futureHitBox.h - 1) / _tileSize;

        bool horizontalCollision = false;
        for (int tileY = topTileY; tileY <= bottomTileY && tileY < mapHeight && tileY >= 0; ++tileY) {
            int mapWidth = static_cast<int>(map[tileY].size());

            const int leftTileX = futureHitBox.x / _tileSize;
            const int rightTileX = (futureHitBox.x + futureHitBox.w - 1) / _tileSize;

            for (int tileX = leftTileX; tileX <= rightTileX && tileX < mapWidth && tileX >= 0; ++tileX) {
                if (map[tileY][tileX] == -1) continue;

                const SDL_Rect tileRect{
                    static_cast<int>(tileX * _tileSize),
                    static_cast<int>(tileY * _tileSize),
                    static_cast<int>(_tileSize),
                    static_cast<int>(_tileSize)
                };

                if (SDL_HasIntersection(&futureHitBox, &tileRect)) {
                    velocity.x = 0.f;
                    horizontalCollision = true;
                    break;
                }
            }

            if (horizontalCollision) break;
        }
    }

    // Check vertical collision
    if (velocity.y != 0.f) {
        SDL_Rect futureHitBox = hitBox;
        futureHitBox.y += static_cast<int>(std::round(velocity.y * deltaTime));

        const int topTileY = futureHitBox.y / _tileSize;
        const int bottomTileY = (futureHitBox.y + futureHitBox.h - 1) / _tileSize;

        bool verticalCollision = false;
        for (int tileY = topTileY; tileY <= bottomTileY && tileY < mapHeight && tileY >= 0; ++tileY) {
            const int mapWidth = static_cast<int>(map[tileY].size());

            const int leftTileX = futureHitBox.x / _tileSize;
            const int rightTileX = (futureHitBox.x + futureHitBox.w - 1) / _tileSize;

            for (int tileX = leftTileX; tileX <= rightTileX && tileX < mapWidth && tileX >= 0; ++tileX) {
                if (map[tileY][tileX] == -1) continue;

                const SDL_Rect tileRect{
                    static_cast<int>(tileX * _tileSize),
                    static_cast<int>(tileY * _tileSize),
                    static_cast<int>(_tileSize),
                    static_cast<int>(_tileSize)
                };

                if (SDL_HasIntersection(&futureHitBox, &tileRect)) {
                    if (velocity.y > 0) {
                        landed = true;
                    }
                    velocity.y = 0.f;
                    verticalCollision = true;
                    break;
                }
            }

            if (verticalCollision) break;
        }
    }

    // Check if entity should stay on ground  
    if (wasOnGround && velocity.y >= 0) {
        auto futureHitBox = hitBox;
        futureHitBox.y += static_cast<int>(std::round(velocity.y * deltaTime));
        
        const int footY = futureHitBox.y + futureHitBox.h;
        const int tileYBelow = footY / _tileSize;

        bool hasGroundBelow = false;
        for (int corner = 0; corner < 2; ++corner) {
            const int cornerX = (corner == 0) ? futureHitBox.x : (futureHitBox.x + futureHitBox.w - 1);
            const int tileX = cornerX / _tileSize;

            if (tileYBelow >= 0 && tileYBelow < mapHeight &&
                tileX >= 0 && tileX < static_cast<int>(map[tileYBelow].size()) &&
                map[tileYBelow][tileX] != -1) {
                hasGroundBelow = true;
                break;
            }
        }

        if (!hasGroundBelow) {
            entity.setOnGround(false);
        }
    }

    if (landed) {
        entity.setOnGround(true);
    }

    entity.setVel(velocity);
}


void PhysicsHandler::onSettingsChanged() {
    const auto oldTileSize = _tileSize;

    _tileSize = _settings.getTileSize();

    const auto tileSizeRatio = (static_cast<float>(oldTileSize) / _tileSize);

    _gAcceleration *= tileSizeRatio;
    _fallGMultiplier *= tileSizeRatio;
}

}