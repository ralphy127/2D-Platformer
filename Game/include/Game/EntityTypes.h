#pragma once

#include <string> 
#include <array>

namespace game {

enum class EntityType {
    // sprite
    PLAYER,
    // texture

    // helpers
    COUNT,
    NONE
};

const std::vector<EntityType> allSimpleTypes = {
    
};

const std::vector<EntityType> allSpriteTypes = {
    EntityType::PLAYER    
};

inline std::string toString(EntityType type) {
    switch (type) {
        case EntityType::PLAYER:
            return std::string("player");
        default:
            return std::string("unknown");
    }
}

}
