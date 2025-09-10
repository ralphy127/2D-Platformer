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
        Idle,
        Walking,
        Running,
        Attack1,
        Attack2,
        Attack3,
        Attack4,
        Jumping,
        Hurt,
        Dead
    };

    void handleDeath() override;

    engine::DynamicSpriteEntity::Config initAndGetConfig(const engine::Settings&) const;

    State _state{State::Idle};

    engine::Clock::Type::time_point _lastBehaviorChange{engine::Clock::getTime()};
    engine::Clock::Type::duration _interval{std::chrono::seconds(4)};
};

}
