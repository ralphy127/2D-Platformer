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
    float deltaTime) const 
{
    auto velocity = entity.getVel();
    auto hitbox = entity.getHitbox();
    auto landed = false;

    checkHorizontalCollision(map, deltaTime, velocity, hitbox);
    checkVerticalCollision(map, deltaTime, velocity, hitbox, landed);
    checkIfStillOnGround(entity, map, deltaTime, hitbox, velocity);

    if (landed)
        entity.setOnGround(true);

    entity.setVel(velocity);
}

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

void PhysicsHandler::handleAttacksCollisions(
    DynamicSpriteEntity& player,
    DynamicEntity& entity) const {
    
    if (&player == &entity)
        throw std::runtime_error("passed two same references (player and entity)");
    
    const auto playerHitbox = player.getHitbox();

    if (player.isAttacking()) {
        const auto playerWeaponHitbox = player.getWeaponHitbox();
        if (!playerWeaponHitbox.has_value())
            throw std::runtime_error("player is attacking, but has no weapon hitbox");
        
        const auto& playerAttackData = player.getCurrentAttackDataView();

        const auto entityHitbox = entity.getHitbox();
        if (SDL_HasIntersectionF(&playerWeaponHitbox.value(), &entityHitbox)) {
            entity.dealDamage(playerAttackData.damage);

            if (playerAttackData.onHit)
                playerAttackData.onHit();
        }
    }

    if (auto* dse = dynamic_cast<DynamicSpriteEntity*>(&entity)) {
        if (dse->isAttacking()) {
            const auto entityWeaponHitbox = dse->getWeaponHitbox();
            if (!entityWeaponHitbox.has_value())
                throw std::runtime_error("entity is attacking, but has no weapon hitbox");

            if (SDL_HasIntersectionF(&entityWeaponHitbox.value(), &playerHitbox)) {
                const auto& attackData = dse->getCurrentAttackDataView();

                player.dealDamage(attackData.damage);

                if (attackData.onHit)
                    attackData.onHit();
            }
        }
    }
}

void PhysicsHandler::checkHorizontalCollision(
    const TileLayer::Grid& map,
    float deltaTime,
    utils::f2v& velocity,
    const SDL_FRect& hitbox) const
{
    if (velocity.x == 0.f)
        return;

    auto futureHitbox = hitbox;
    futureHitbox.x += static_cast<int>(std::round(velocity.x * deltaTime));

    const int topTileY = futureHitbox.y / _tileSize;
    const int bottomTileY = (futureHitbox.y + futureHitbox.h - 1) / _tileSize;
    const int mapHeight = static_cast<int>(map.size());

    for (int tileY = topTileY; tileY <= bottomTileY && tileY < mapHeight && tileY >= 0; ++tileY) {
        int mapWidth = static_cast<int>(map[tileY].size());

        const int leftTileX = futureHitbox.x / _tileSize;
        const int rightTileX = (futureHitbox.x + futureHitbox.w - 1) / _tileSize;

        for (int tileX = leftTileX;
             tileX <= rightTileX && tileX < mapWidth && tileX >= 0; 
             ++tileX) {

            if (map[tileY][tileX] == -1) continue;

            SDL_FRect tileRect{
                static_cast<float>(tileX * _tileSize),
                static_cast<float>(tileY * _tileSize),
                static_cast<float>(_tileSize),
                static_cast<float>(_tileSize)
            };

            if (SDL_HasIntersectionF(&futureHitbox, &tileRect)) {
                velocity.x = 0.f;
                return;
            }
        }
    }
}

void PhysicsHandler::checkVerticalCollision(
    const TileLayer::Grid& map,
    float deltaTime,
    utils::f2v& velocity,
    const SDL_FRect& hitbox,
    bool& landed) const
{
    if (velocity.y == 0.f)
        return;

    auto futureHitbox = hitbox;
    futureHitbox.y += static_cast<int>(std::round(velocity.y * deltaTime));

    const int topTileY = futureHitbox.y / _tileSize;
    const int bottomTileY = (futureHitbox.y + futureHitbox.h - 1) / _tileSize;
    const int mapHeight = static_cast<int>(map.size());

    for (int tileY = topTileY; tileY <= bottomTileY && tileY < mapHeight && tileY >= 0; ++tileY) {
        const int mapWidth = static_cast<int>(map[tileY].size());

        const int leftTileX = futureHitbox.x / _tileSize;
        const int rightTileX = (futureHitbox.x + futureHitbox.w - 1) / _tileSize;

        for (int tileX = leftTileX;
             tileX <= rightTileX && tileX < mapWidth && tileX >= 0;
             ++tileX) {

            if (map[tileY][tileX] == -1) continue;

            SDL_FRect tileRect{
                static_cast<float>(tileX * _tileSize),
                static_cast<float>(tileY * _tileSize),
                static_cast<float>(_tileSize),
                static_cast<float>(_tileSize)
            };

            if (SDL_HasIntersectionF(&futureHitbox, &tileRect)) {
                if (velocity.y > 0.f) {
                    landed = true;
                }
                velocity.y = 0.f;
                return;
            }
        }
    }
}

void PhysicsHandler::checkIfStillOnGround(
    DynamicEntity& entity,
    const TileLayer::Grid& map,
    float deltaTime,
    const SDL_FRect& hitbox,
    const utils::f2v& velocity) const
{
    if (velocity.y < 0.f)
        return;

    auto futureHitbox = hitbox;
    futureHitbox.y += static_cast<int>(std::round(velocity.y * deltaTime));

    const int footY = futureHitbox.y + futureHitbox.h;
    const int tileYBelow = footY / _tileSize;
    const int mapHeight = static_cast<int>(map.size());

    for (int corner = 0; corner < 2; ++corner) {
        const int cornerX = (corner == 0) ? futureHitbox.x : (futureHitbox.x + futureHitbox.w - 1);
        const int tileX = cornerX / _tileSize;

        if (tileYBelow >= 0 && tileYBelow < mapHeight &&
            tileX >= 0 && tileX < static_cast<int>(map[tileYBelow].size()) &&
            map[tileYBelow][tileX] != -1) {
            return;
        }
    }

    entity.setOnGround(false);
}

void PhysicsHandler::onSettingsChanged() {
    const auto oldTileSize = _tileSize;

    _tileSize = _settings.getTileSize();

    const auto tileSizeRatio = (static_cast<float>(oldTileSize) / _tileSize);

    _gAcceleration *= tileSizeRatio;
    _fallGMultiplier *= tileSizeRatio;
}

}