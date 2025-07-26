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

bool PhysicsHandler::AABBcast(const SDL_Rect& source, const SDL_Rect& target, const utils::f2v& vel, collisionHit& outhit, float deltaTime) const {
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

void PhysicsHandler::handleMapCollisions(DynamicEntity& entity, const TileLayer::Grid& map, float deltaTime) const {
    auto vel = entity.getVel();
    auto hitBox = entity.getHitBox();
    bool wasOnGround = entity.isOnGround();
    bool landed = false;

    int moveX = (vel.x != 0.f) ? std::max(1, static_cast<int>(std::round(std::abs(vel.x * deltaTime)))) : 0;
    int stepX = (vel.x > 0) ? 1 : (vel.x < 0 ? -1 : 0);
    for (int i = 0; i < moveX; ++i) {
        if (stepX == 0) break;
        hitBox.x += stepX;
        bool collision = false;
        for (int y = (hitBox.y / _tileSize); y <= (hitBox.y + hitBox.h - 1) / _tileSize && y < static_cast<int>(map.size()); ++y) {
            for (int x = (hitBox.x / _tileSize); x <= (hitBox.x + hitBox.w - 1) / _tileSize && x < static_cast<int>(map[y].size()); ++x) {
                if (map[y][x] == -1) continue;
                SDL_Rect tileRect{
                    static_cast<int>(x * _tileSize),
                    static_cast<int>(y * _tileSize),
                    static_cast<int>(_tileSize),
                    static_cast<int>(_tileSize)
                };
                if (SDL_HasIntersection(&hitBox, &tileRect)) {
                    if (stepX > 0) hitBox.x = tileRect.x - hitBox.w;
                    else if (stepX < 0) hitBox.x = tileRect.x + tileRect.w;
                    vel.x = 0.f;
                    collision = true;
                    break;
                }
            }
            if (collision) break;
        }
        if (collision) break;
    }

    int moveY = (vel.y != 0.f) ? std::max(1, static_cast<int>(std::round(std::abs(vel.y * deltaTime)))) : 0;
    int stepY = (vel.y > 0) ? 1 : (vel.y < 0 ? -1 : 0);
    for (int i = 0; i < moveY; ++i) {
        if (stepY == 0) break;
        hitBox.y += stepY;
        bool collision = false;
        for (int y = (hitBox.y / _tileSize); y <= (hitBox.y + hitBox.h - 1) / _tileSize && y < static_cast<int>(map.size()); ++y) {
            for (int x = (hitBox.x / _tileSize); x <= (hitBox.x + hitBox.w - 1) / _tileSize && x < static_cast<int>(map[y].size()); ++x) {
                if (map[y][x] == -1) continue;
                SDL_Rect tileRect{
                    static_cast<int>(x * _tileSize),
                    static_cast<int>(y * _tileSize),
                    static_cast<int>(_tileSize),
                    static_cast<int>(_tileSize)
                };
                if (SDL_HasIntersection(&hitBox, &tileRect)) {
                    if (stepY > 0) {
                        hitBox.y = tileRect.y - hitBox.h;
                        landed = true;
                    } else if (stepY < 0) {
                        hitBox.y = tileRect.y + tileRect.h;
                    }
                    vel.y = 0.f;
                    collision = true;
                    break;
                }
            }
            if (collision) break;
        }
        if (collision) break;
    }

    if (wasOnGround && vel.y >= 0) {
        bool hasGroundBelow = false;
        
        for (int corner = 0; corner < 2; ++corner) {
            int checkX = (corner == 0) ? hitBox.x : (hitBox.x + hitBox.w - 1);
            int checkY = hitBox.y + hitBox.h;
            
            int tileX = checkX / _tileSize;
            int tileY = checkY / _tileSize;
            
            if (tileY >= 0 && tileY < static_cast<int>(map.size()) &&
                tileX >= 0 && tileX < static_cast<int>(map[tileY].size()) &&
                map[tileY][tileX] != -1) {
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

    entity.setVel(vel);
    entity.setPos({static_cast<float>(hitBox.x), static_cast<float>(hitBox.y)});
}

void PhysicsHandler::onSettingsChanged() {
    const auto oldTileSize = _tileSize;

    _tileSize = _settings.getTileSize();

    const auto tileSizeRatio = (static_cast<float>(oldTileSize) / _tileSize);

    _gAcceleration *= tileSizeRatio;
    _fallGMultiplier *= tileSizeRatio;
}

}