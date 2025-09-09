#pragma once

#include <unordered_map>
#include <Engine/DynamicSpriteEntity.h>
#include <Engine/EventHandler.h>

namespace game {

class Player : public engine::DynamicSpriteEntity {
public:
    Player(engine::Settings&, engine::ISpriteTextures&, const engine::EventHandler&);

    void update(float deltaTime) override;

    void render(SDL_Renderer&, engine::Camera&) const override;

    void logDebugState();

private:
    enum class State {
        Idle,
        Walking,
        Running,
        Attack1,
        Attack2,
        Attack3,
        Protecting,
        Jumping,
        Hurt,
        Dead
    };

    engine::DynamicSpriteEntity::Config initAndGetConfig(const engine::Settings&) const;

    void createAndAddAttacks();

    /// @brief Handles player movement and updates velocity and state accordingly.
    void handleMovement(float deltaTime);

    /// @brief Handles sprinting state based on whether shift key is pressed.
    void handleLShift(bool shiftPressed, engine::SpriteData& spriteData);
    
    /// @brief Changes state and animation to desired if possible
    void tryToChangeAnimation(State, engine::SpriteData&);

    void handleAttacks();

    std::string stateToString(State);
    
    const engine::EventHandler& _eventHandler; ///< Reference to input event handler.
    bool _jumpPressedLastFrame{false};         ///< Jumping guard
    State _state{State::Idle};                 ///< Current player state.
};

}
