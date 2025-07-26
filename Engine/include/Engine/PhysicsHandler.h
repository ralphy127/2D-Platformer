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

    PhysicsHandler(const PhysicsHandler&) = delete;
    PhysicsHandler& operator=(const PhysicsHandler&) = delete;
    PhysicsHandler(PhysicsHandler&&) = delete;
    PhysicsHandler& operator=(PhysicsHandler&&) = delete;

    bool AABBcast(const SDL_Rect& source, const SDL_Rect& target, const utils::f2v& velocity, collisionHit& outhit, float deltaTime) const;

    void applyGravity(DynamicEntity&, float deltaTime) const;

    void setGAcceleration(float g) { _gAcceleration = g; }

    void handleMapCollisions(DynamicEntity& entity, const TileLayer::Grid& map, float deltaTime) const;

    void setFallGMultiplier(float fallGMultiplier) { _fallGMultiplier = fallGMultiplier; }

    void onSettingsChanged() override;
private:
    Settings& _settings;

    size_t _tileSize{};
    float _gAcceleration{};
    float _fallGMultiplier{};
};

}