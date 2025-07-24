#pragma once

#include <string> 
#include <array>

namespace game {



/// @brief Represents different types of game entities.
enum class EntityType {
    // sprite
    PLAYER,
    // texture

    // helpers
    COUNT,
    NONE
};

/// @brief List of all simple (non-sprite) entity types.
const std::vector<EntityType> allSimpleTypes = {
    
};

/// @brief List of all sprite-based entity types.
const std::vector<EntityType> allSpriteTypes = {
    EntityType::PLAYER    
};

/// @brief Converts an EntityType enum to a lowercase string representation.
inline std::string toString(EntityType type) {
    switch (type) {
        case EntityType::PLAYER:
            return std::string("player");
        default:
            return std::string("unknown");
    }
}

}
