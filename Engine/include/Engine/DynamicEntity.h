#pragma once

#include "Engine/Drawable.h"
#include "Engine/IUpdatable.h"

namespace engine {

/// @brief Abstract class representing movable and updatable entity
class DynamicEntity : public Drawable, public IUpdatable {
public:
    /// @brief Configuration struct used to initialize a DynamicEntity.
    struct Config : Drawable::Config {
        float maxHealth;     ///< Maximum health points.
        float health;        ///< Current health points.
        float defaultSpeed;  ///< Default movement speed.
        float sprintSpeed;   ///< Speed used when sprinting.
        float defaultJumpVy; ///< Default initial vertical jumping speed.
    };

    DynamicEntity(Settings&, const Config&);

    /// @brief Updates entity's logic based on the elapsed time.
    void update(float deltaTime) override;

    /// @brief Updates position based on current velocity.
    void applyMovement(float deltaTime);

    utils::f2v getVel() const { return _vel; }
    void setVel(utils::f2v vel) { _vel = vel; }

    bool isOnGround() const { return _onGround; }
    void setOnGround(bool onGround) { _onGround = onGround; }

    float getHealth() const { return _health; }
    void setHealth(float health) { capHealthIfNeeded(health); }
    void changeHealth(float health) { capHealthIfNeeded(_health + health); }
    void setHealthToFull() { _health = _maxHealth; }
    void dealDamage(float damage) { capHealthIfNeeded(_health - damage); }
    void heal(float health) { capHealthIfNeeded(_health + health); }

    float getMaxHealth() const { return _maxHealth; }
    void setMaxHealth(float maxHealth) { _maxHealth = maxHealth; capHealthIfNeeded(_health); }

    bool isDead() const { return _health <= 0.f; }
    bool isCritical() const { return _health > 0 && _health <= _maxHealth / 3; }
    bool isWounded() const { return _health > _maxHealth / 3 && _health <= _maxHealth / 3 * 2; }
    bool isHealthy() const { return _health > _maxHealth / 3 * 2; }

    float getDefaultSpeed() const { return _defaultSpeed; }
    void setDefaultSpeed(float speed) { if (speed > 0.f) _defaultSpeed = speed; }

protected:
    /// @brief Renders the entity's main hitbox (red rectangle).
    void renderEntityHitbox(SDL_Renderer&, Camera&) const;

    /// @brief Renders the entity's texture hitbox (grey rectangle).
    void renderTextureHitbox(SDL_Renderer&, Camera&, const SDL_Rect& textureRect) const;
    
    /// @brief Moves the entity based on direction and default speed.
    void move() { _vel.x = getDirection() * _defaultSpeed; }

    /// @brief Moves the entity based on direction and sprint speed.
    void sprint() { _vel.x = getDirection() * _sprintSpeed; }

    /// @brief Stops horizontal movement.
    void stop() { _vel.x = 0.f; }

    /// @brief Jumps if possible with default jumping speed.
    void jump();
private:
    /// @brief Makes sure health is between 0 and max health.
    void capHealthIfNeeded(float health);

    float _health{};           ///< Current health.
    float _maxHealth;          ///< Maximum health.
    float _defaultSpeed;       ///< Normal movement speed.
    float _sprintSpeed;        ///< Sprint movement speed.
    float _defaultJumpVy;      ///< Initial vertical jumping speed.
    utils::f2v _vel{};         ///< Current velocity.
    bool _onGround{};          ///< True if entity is on ground
};

}
