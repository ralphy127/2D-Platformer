#pragma once

#include <unordered_map>
#include <Engine/DynamicSpriteEntity.h>

namespace game {

class Player : public engine::DynamicSpriteEntity {
public:
    using DynamicSpriteEntity::update;
    Player(engine::Settings&, engine::ISpriteTextures&);

    void update(float deltaTime, const std::unordered_map<SDL_Keycode, bool>& keyStates);

    void render(SDL_Renderer& renderer, engine::Camera& camera) const override;
private:
    enum class State { IDLE, WALKING, RUNNING, ATTACK1, ATTACK2, ATTACK3, PROTECTING, JUMPING, HURT, DEAD };

    DynamicSpriteEntity::Config initAndGetConfig(engine::Settings&) const;

    State _state{State::IDLE};
};

}
