#pragma once

#include "Engine/Drawable.h"
#include "Engine/IUpdatable.h"

namespace engine {

class DynamicEntity : public Drawable, IUpdatable {
public:
    struct Config : Drawable::Config {
        float maxHealth;
        float health;
    };

    DynamicEntity(Settings&, const Config&);

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
    
    bool isUpdatable() const override { return true; }

protected:
    void capHealthIfNeeded(float health) { health > _maxHealth ? _health = _maxHealth : _health = health; }

private:
    float _health{};
    float _maxHealth;
};

}
