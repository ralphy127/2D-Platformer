#pragma once

#include <chrono>
#include <Engine/DynamicSpriteEntity.h>
#include <Engine/Clock.h>

namespace game {

class Archer : public engine::DynamicSpriteEntity {
public:
    Archer(engine::Settings&, engine::ISpriteTextures&);

    void update(float deltaTime) override;

private:
    enum class State {
        IDLE,       ///< Standing still.
        WALKING,    ///< Moving at walking speed.
        RUNNING,    ///< Moving at sprint speed.
        ATTACK1,    ///< Performing attack animation 1.
        ATTACK2,    ///< Performing attack animation 2.
        ATTACK3,    ///< Performing attack animation 3.
        ATTACK4,    ///< Performing attack animation 4.
        JUMPING,    ///< In the air.
        HURT,       ///< Taking damage.
        DEAD        ///< Player is dead.
    };

    engine::DynamicSpriteEntity::Config initAndGetConfig(const engine::Settings&) const;

    State _state{State::IDLE};

    engine::Clock::Type::time_point _lastBehaviorChange{engine::Clock::getTime()};
    engine::Clock::Type::duration _interval{std::chrono::seconds(4)};
};

}