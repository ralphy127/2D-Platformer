#pragma once

#include <chrono>
#include <Engine/DynamicSpriteEntity.h>

namespace game {

class Commander : public engine::DynamicSpriteEntity {
public:
    using ClockType = std::chrono::steady_clock;

    Commander(engine::Settings&, engine::ISpriteTextures&);

    void update(float deltaTime) override;

private:
    enum class State {
        IDLE,       ///< Standing still.
        WALKING,    ///< Moving at walking speed.
        RUNNING,    ///< Moving at sprint speed.
        ATTACK1,    ///< Performing attack animation 1.
        ATTACK2,    ///< Performing attack animation 2.
        ATTACK3,    ///< Performing attack animation 3.
        PROTECTING, ///< Defensive state.
        JUMPING,    ///< In the air.
        HURT,       ///< Taking damage.
        DEAD        ///< Player is dead.
    };

    engine::DynamicSpriteEntity::Config initAndGetConfig(const engine::Settings&) const;

    State _state{State::IDLE};

    ClockType::time_point _lastBehaviorChange{ClockType::now()};
    ClockType::duration _interval{std::chrono::seconds(3)};
};

}