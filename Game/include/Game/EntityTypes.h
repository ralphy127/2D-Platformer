#pragma once

#include <string> 
#include <array>

namespace game {

/// @brief Represents different types of game entities.
enum class EntityType {
    // sprite
    PLAYER,
    COMMANDER,
    ARCHER,
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
    EntityType::PLAYER,
    EntityType::COMMANDER,
    EntityType::ARCHER,
};

/// @brief Converts an EntityType enum to a lowercase string representation.
inline std::string toString(EntityType type) {
    switch (type) {
        case EntityType::PLAYER:
            return std::string("player");
        case EntityType::COMMANDER:
            return std::string("commander");
        case EntityType::ARCHER:
            return std::string("archer");
        default:
            return std::string("unknown");
    }
}

}
