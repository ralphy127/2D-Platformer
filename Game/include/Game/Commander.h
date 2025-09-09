#pragma once

#include <chrono>
#include <Engine/DynamicSpriteEntity.h>
#include <Engine/Clock.h>

namespace game {

class Commander : public engine::DynamicSpriteEntity {
public:
    Commander(engine::Settings&, engine::ISpriteTextures&);

    void update(float deltaTime) override;

private:
    enum class State {
        Idle,
        Walking,
        Running,
        Attack1,
        Attack2,
        Attack3,
        Jumping,
        Hurt,
        Dead
    };

    engine::DynamicSpriteEntity::Config initAndGetConfig(const engine::Settings&) const;

    State _state{State::Idle};

    engine::Clock::Type::time_point _lastBehaviorChange{engine::Clock::getTime()};
    engine::Clock::Type::duration _interval{std::chrono::seconds(3)};
};

}
