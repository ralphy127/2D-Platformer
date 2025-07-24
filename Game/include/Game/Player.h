#pragma once

#include <unordered_map>
#include <Engine/DynamicSpriteEntity.h>
#include <Engine/EventHandler.h>

namespace game {

/// @brief Represents the player character in the game.
class Player : public engine::DynamicSpriteEntity {
public:
    using DynamicSpriteEntity::update;
    using Base = engine::DynamicSpriteEntity;

    /// @brief Constructs a Player instance.
    /// @param settings Reference to global game settings.
    /// @param spriteTextures Reference to sprite texture provider.
    /// @param eventHandler Reference to event handler to process input.
    Player(engine::Settings&, engine::ISpriteTextures&, const engine::EventHandler&);

    /// @brief Updates player state and animation each frame.
    /// @param deltaTime Time elapsed since last frame.
    void update(float deltaTime) override;

    /// @brief Renders the player sprite to the screen.
    /// @param renderer SDL renderer.
    /// @param camera Camera for converting world to screen coordinates.
    void render(SDL_Renderer& renderer, engine::Camera& camera) const override;

private:
    /// @brief Defines various player states controlling animations and behavior.
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

    /// @brief Initializes and returns the configuration struct for the base DynamicSpriteEntity.
    /// @param settings Reference to global game settings.
    /// @return Config object with player-specific settings.
    Base::Config initAndGetConfig(engine::Settings&) const;

    /// @brief Handles player movement and updates velocity and state accordingly.
    /// @param deltaTime Time elapsed since last frame.
    void handleMovement(float deltaTime);

    /// @brief Manages sprinting state based on whether shift key is pressed.
    /// @param shiftPressed Whether the shift key is currently pressed.
    /// @param spriteData Reference to the sprite animation data to update.
    void handleLShift(bool shiftPressed, engine::SpriteData& spriteData);

    const engine::EventHandler& _eventHandler; ///< Reference to input event handler.

    State _state{State::IDLE};                 ///< Current player state.
};

}
