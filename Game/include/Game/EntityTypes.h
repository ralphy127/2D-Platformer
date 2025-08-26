#pragma once

#include <string> 
#include <array>

namespace game {

/// @brief Represents different types of game entities.
enum class EntityType {
    NONE,

    PLAYER,
    COMMANDER,
    ARCHER,
    SPRITE_END,

    SIMPLE_END,
};

constexpr size_t simpleCount = static_cast<size_t>(EntityType::SIMPLE_END) -
                               static_cast<size_t>(EntityType::SPRITE_END) - 1UL;
                              
constexpr std::array<EntityType, simpleCount> allSimpleTypes = {

};

constexpr size_t spriteCount = static_cast<size_t>(EntityType::SPRITE_END) - 1UL;

constexpr std::array<EntityType, spriteCount> allSpriteTypes = {
    EntityType::PLAYER,
    EntityType::COMMANDER,
    EntityType::ARCHER,
};

inline std::string toString(EntityType type) {
    switch (type) {
        case EntityType::PLAYER:
            return std::string("player");
        case EntityType::COMMANDER:
            return std::string("commander");
        case EntityType::ARCHER:
            return std::string("archer");
        case EntityType::NONE:
        case EntityType::SIMPLE_END:
        case EntityType::SPRITE_END:
            throw std::runtime_error("wrong entity type (helper)");
        default:
            return std::string("unknown");
    }
}

}
