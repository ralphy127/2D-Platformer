#pragma once

#include "Engine/Drawable.h"
#include "Engine/IUpdatable.h"

namespace engine {

class DynamicEntity : public Drawable, public IUpdatable {
public:
    struct Config : Drawable::Config {
        float maxHealth;
        float health;
        float defaultSpeed;
        float sprintSpeed;
    };

    DynamicEntity(Settings&, const Config&);

    void update(float deltaTime) override;

protected:
    void setHealth(float health) { capHealthIfNeeded(health); }
    void changeHealth(float health) { capHealthIfNeeded(_health + health); }
    void setHealthToFull() { _health = _maxHealth; }
    float getHealth() const { return _health; }

    float getMaxHealth() const { return _maxHealth; }
    void setMaxHealth(float maxHealth) { _maxHealth = maxHealth; capHealthIfNeeded(_health); }

    bool isDead() const { return _health <= 0.f; }
    bool isCritical() const { return _health > 0 && _health <= _maxHealth / 3; }
    bool isWounded() const { return _health > _maxHealth / 3 && _health <= _maxHealth / 3 * 2; }
    bool isHealthy() const { return _health > _maxHealth / 3 * 2; }    

protected:
    void capHealthIfNeeded(float health) { health > _maxHealth ? _health = _maxHealth : _health = health; }

    utils::f2v getVel() const { return _vel; }
    void setVel(utils::f2v vel) { _vel = vel; }

    float getDefaultSpeed() const { return _defaultSpeed; }
    void setDefaultSpeed(float defaultSpeed) { if(defaultSpeed > 0.f) _defaultSpeed = defaultSpeed; }

    void move() { _vel.x = getDirection() * _defaultSpeed; }
    void sprint() { _vel.x = getDirection() * _sprintSpeed; }
    void stop() { _vel.x = 0.f; }
    
private:
    float _health{};
    float _maxHealth;
    float _defaultSpeed;
    float _sprintSpeed;
    utils::f2v _vel{0.f, 0.f};
};

}
