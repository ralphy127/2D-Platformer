#pragma once

#include <chrono>
#include <Engine/DynamicSpriteEntity.h>
#include <Engine/Clock.h>

namespace game {

class Archer : public engine::DynamicSpriteEntity {
public:
    Archer(engine::Settings&, engine::ISpriteTextures&);

    /// @brief Updates archer AI behavior and animation state.
    void update(float deltaTime) override;

private:
    /// @brief Defines various archer states controlling animations and behavior.
    enum class State {
        IDLE,
        WALKING,
        RUNNING,
        ATTACK1,
        ATTACK2,
        ATTACK3,
        ATTACK4,
        JUMPING,
        HURT,
        DEAD
    };

    engine::DynamicSpriteEntity::Config initAndGetConfig(const engine::Settings&) const;

    State _state{State::IDLE};

    engine::Clock::Type::time_point _lastBehaviorChange{engine::Clock::getTime()};
    engine::Clock::Type::duration _interval{std::chrono::seconds(4)};
};

}
