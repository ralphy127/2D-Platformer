#pragma once

#include <Engine/DynamicEntity.h>
#include <Engine/Settings.h>

namespace engine {

struct collisionHit {
    utils::f2v point;
    utils::f2v normal;
    float distance;
};

using Collision = std::pair<bool, bool>;

class PhysicsHandler : public ISettingsObserver {
public:
    PhysicsHandler(Settings&);

    ~PhysicsHandler();

    PhysicsHandler(const PhysicsHandler&) = delete;
    PhysicsHandler& operator=(const PhysicsHandler&) = delete;
    PhysicsHandler(PhysicsHandler&&) = delete;
    PhysicsHandler& operator=(PhysicsHandler&&) = delete;

    Collision AABBcast(const SDL_Rect& source, const SDL_Rect& target, const utils::f2v& velocity, collisionHit& outhit, float deltaTime) const;

    void applyGravity(DynamicEntity& entity, float deltaTime);

    void setGAcceleration(float g) { _gAcceleration = g; }

    void setFallGMultiplier(float fallGMultiplier) { _fallGMultiplier = fallGMultiplier; }

    void onSettingsChanged() override;
private:
    Settings& _settings;

    size_t _tileSize{};
    float _gAcceleration{};
    float _fallGMultiplier{};
};

}