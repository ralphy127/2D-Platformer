#pragma once

#include "Engine/TileLayer.h"
#include "Engine/Settings.h"
#include "Engine/DynamicSpriteEntity.h"

namespace engine {

/// @brief Contains collision detection result data.
struct collisionHit {
    utils::f2v point;   ///< Contact point of collision.
    utils::f2v normal;  ///< Surface normal at collision point.
    float distance; ///< Distance to collision point.
};

class PhysicsHandler : public ISettingsObserver {
public:
    PhysicsHandler(Settings&);
    ~PhysicsHandler();

    /// @brief Applies gravity force to entity based on ground state and delta time.
    void applyGravity(DynamicEntity&, float deltaTime) const;

    void setGAcceleration(float g) { _gAcceleration = g; }

    /// @brief Handles collisions between entity and map tiles.
    void handleMapCollisions(DynamicEntity&, const TileLayer::Grid&, float deltaTime) const;

    /// @brief Handles attack collisions between player and an entity.
    void handleAttacksCollisions(DynamicSpriteEntity& player, DynamicEntity& entity) const;
    
    void setFallGMultiplier(float fallGMultiplier) { _fallGMultiplier = fallGMultiplier; }

    void onSettingsChanged() override;

private:
    /// @brief Performs AABB collision detection with movement prediction.
    bool AABBcast(
        const SDL_Rect& source,
        const SDL_Rect& target,
        const utils::f2v& velocity,
        collisionHit& outhit,
        float deltaTime) const;

    /// @brief Checks for horizontal collisions with map tiles.
    void checkHorizontalCollision(
        const TileLayer::Grid&, 
        float deltaTime,
        utils::f2v& velocity,
        const SDL_FRect& hitBox) const;
    
    /// @brief Checks for vertical collisions with map tiles.
    void checkVerticalCollision(
        const TileLayer::Grid&,
        float deltaTime,
        utils::f2v& velocity,
        const SDL_FRect& hitBox,
        bool& landed) const;

    /// @brief Checks if entity should remain on ground or start falling.
    void checkIfStillOnGround(
        DynamicEntity& entity,
        const TileLayer::Grid&,
        float deltaTime,
        const SDL_FRect& hitBox,
        const utils::f2v& velocity) const;

    Settings& _settings;          ///< Reference to game settings.

    uint32_t _tileSize{};         ///< Size of map tiles in pixels.
    float _gAcceleration{};   ///< Gravitational acceleration value.
    float _fallGMultiplier{}; ///< Multiplier for faster falling.
};

}