#pragma once

#include <Engine/TileLayer.h>
#include <Engine/Settings.h>

namespace engine {

struct collisionHit {
    utils::f2v point;
    utils::f2v normal;
    float distance;
};

class DynamicEntity;

class PhysicsHandler : public ISettingsObserver {
public:
    PhysicsHandler(Settings&);

    ~PhysicsHandler();

    bool AABBcast(
        const SDL_Rect& source,
        const SDL_Rect& target,
        const utils::f2v& velocity,
        collisionHit& outhit,
        float deltaTime) const;

    void applyGravity(DynamicEntity&, float deltaTime) const;

    void setGAcceleration(float g) { _gAcceleration = g; }

    void handleMapCollisions(DynamicEntity&, const TileLayer::Grid&, float deltaTime) const;

    void setFallGMultiplier(float fallGMultiplier) { _fallGMultiplier = fallGMultiplier; }

    void onSettingsChanged() override;

private:
    void checkHorizontalCollision(
        const TileLayer::Grid&, 
        float deltaTime,
        utils::f2v& velocity,
        const SDL_Rect& hitBox) const;
    
    void checkVerticalCollision(
        const TileLayer::Grid&,
        float deltaTime,
        utils::f2v& velocity,
        const SDL_Rect& hitBox,
        bool& landed) const;

    void checkIfStillOnGround(
        DynamicEntity& entity,
        const TileLayer::Grid&,
        float deltaTime,
        const SDL_Rect& hitBox,
        const utils::f2v& velocity) const;


    Settings& _settings;

    size_t _tileSize{};
    float _gAcceleration{};
    float _fallGMultiplier{};
};

}