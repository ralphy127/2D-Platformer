#pragma once

#include "Engine/Drawable.h"
#include "Engine/IUpdatable.h"

namespace engine {

/// @brief Abstract class representing a movable and damageable entity.
class DynamicEntity : public Drawable, public IUpdatable {
public:
    /// @brief Configuration struct used to initialize a DynamicEntity.
    struct Config : Drawable::Config {
        float maxHealth;     ///< Maximum health points.
        float health;        ///< Current health points.
        float defaultSpeed;  ///< Default movement speed.
        float sprintSpeed;   ///< Speed used when sprinting.
        float defaultJumpVy;
    };

    /// @brief Constructs a DynamicEntity using provided settings and config.
    DynamicEntity(Settings&, const Config&);

    /// @brief Updates the entity logic every frame.
    /// @param deltaTime Time elapsed since last update.
    void update(float deltaTime) override;

    void applyMovement(float deltaTime);

    /// @brief Gets current velocity vector.
    utils::f2v getVel() const { return _vel; }

    /// @brief Sets current velocity vector.
    void setVel(utils::f2v vel) { _vel = vel; }

    /// @brief Checks if entity is currently on the ground.
    /// @return True if entity is on the ground, false otherwise.
    bool isOnGround() const { return _onGround; }

    /// @brief Sets the on-ground state of the entity.
    /// @param onGround True if entity is on the ground, false otherwise.
    void setOnGround(bool onGround) { _onGround = onGround; }

protected:
    /// @brief Sets health value, capped at maxHealth.
    void setHealth(float health) { capHealthIfNeeded(health); }

    /// @brief Changes current health by a delta (positive or negative), capped at maxHealth.
    void changeHealth(float health) { capHealthIfNeeded(_health + health); }

    /// @brief Sets health to maxHealth.
    void setHealthToFull() { _health = _maxHealth; }

    /// @brief Gets the current health value.
    float getHealth() const { return _health; }

    /// @brief Gets max health.
    float getMaxHealth() const { return _maxHealth; }

    /// @brief Sets max health and ensures current health stays within limits.
    void setMaxHealth(float maxHealth) { _maxHealth = maxHealth; capHealthIfNeeded(_health); }

    /// @brief Checks if entity is dead.
    bool isDead() const { return _health <= 0.f; }

    /// @brief Checks if entity is in critical state (<= 1/3 health).
    bool isCritical() const { return _health > 0 && _health <= _maxHealth / 3; }

    /// @brief Checks if entity is wounded (between 1/3 and 2/3 health).
    bool isWounded() const { return _health > _maxHealth / 3 && _health <= _maxHealth / 3 * 2; }

    /// @brief Checks if entity is healthy (> 2/3 health).
    bool isHealthy() const { return _health > _maxHealth / 3 * 2; }

    /// @brief Clamps health to be within [0, maxHealth].
    void capHealthIfNeeded(float health) { health > _maxHealth ? _health = _maxHealth : _health = health; }

    /// @brief Gets default movement speed.
    float getDefaultSpeed() const { return _defaultSpeed; }

    /// @brief Sets default movement speed (must be > 0).
    void setDefaultSpeed(float defaultSpeed) { if (defaultSpeed > 0.f) _defaultSpeed = defaultSpeed; }

    /// @brief Moves the entity based on direction and default speed.
    void move() { _vel.x = getDirection() * _defaultSpeed; }

    /// @brief Moves the entity based on direction and sprint speed.
    void sprint() { _vel.x = getDirection() * _sprintSpeed; }

    /// @brief Stops horizontal movement.
    void stop() { _vel.x = 0.f; }

    void jump();

private:
    float _health{};           ///< Current health.
    float _maxHealth;          ///< Maximum health.
    float _defaultSpeed;       ///< Normal movement speed.
    float _sprintSpeed;        ///< Sprint movement speed.
    float _defaultJumpVy;
    utils::f2v _vel{0.f, 0.f}; ///< Current velocity vector.
    bool _onGround{false};     ///< True if entity is on ground
};

}
