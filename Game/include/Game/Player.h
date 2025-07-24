#pragma once

#include <unordered_map>
#include <Engine/DynamicSpriteEntity.h>
#include <Engine/EventHandler.h>

namespace game {

class Player : public engine::DynamicSpriteEntity {
public:
    using DynamicSpriteEntity::update;
    using Base = engine::DynamicSpriteEntity;

    Player(engine::Settings&, engine::ISpriteTextures&, const engine::EventHandler&);

    void update(float deltaTime) override;

    void render(SDL_Renderer& renderer, engine::Camera& camera) const override;

private:
    enum class State { IDLE, WALKING, RUNNING, ATTACK1, ATTACK2, ATTACK3, PROTECTING, JUMPING, HURT, DEAD };

    Base::Config initAndGetConfig(engine::Settings&) const;
    
    void handleMovement(float deltaTime);
    
    void handleLShift(bool shiftPressed, engine::SpriteData& spriteData);

    const engine::EventHandler& _eventHandler;

    State _state{State::IDLE};
};

}
