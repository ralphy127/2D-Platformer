#include "Engine/DynamicEntity.h"

namespace engine {

DynamicEntity::DynamicEntity(Settings& settings, const Config& config) 
    : Drawable(settings, config),
      _maxHealth(config.maxHealth) {

    capHealthIfNeeded(_maxHealth);
}

}
