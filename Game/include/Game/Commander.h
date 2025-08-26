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
        IDLE,
        WALKING,
        RUNNING,
        ATTACK1,
        ATTACK2,
        ATTACK3,
        PROTECTING,
        JUMPING,
        HURT,
        DEAD
    };

    engine::DynamicSpriteEntity::Config initAndGetConfig(const engine::Settings&) const;

    State _state{State::IDLE};

    engine::Clock::Type::time_point _lastBehaviorChange{engine::Clock::getTime()};
    engine::Clock::Type::duration _interval{std::chrono::seconds(3)};
};

}
