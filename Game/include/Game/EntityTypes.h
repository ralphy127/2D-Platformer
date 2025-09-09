#pragma once

#include <string> 
#include <array>

namespace game {

/// @brief Represents different types of game entities.
enum class EntityType {
    None,

    Player,
    Commander,
    Archer,
    SpriteEnd,

    SimpleEnd,
};

constexpr size_t simpleCount = static_cast<size_t>(EntityType::SimpleEnd) -
                               static_cast<size_t>(EntityType::SpriteEnd) - 1UL;
                              
constexpr std::array<EntityType, simpleCount> allSimpleTypes = {

};

constexpr size_t spriteCount = static_cast<size_t>(EntityType::SpriteEnd) - 1UL;

constexpr std::array<EntityType, spriteCount> allSpriteTypes = {
    EntityType::Player,
    EntityType::Commander,
    EntityType::Archer,
};

inline std::string toString(EntityType type) {
    switch (type) {
        case EntityType::Player:
            return std::string("Player");
        case EntityType::Commander:
            return std::string("Commander");
        case EntityType::Archer:
            return std::string("Archer");
        case EntityType::None:
        case EntityType::SimpleEnd:
        case EntityType::SpriteEnd:
            throw std::runtime_error("wrong entity type (helper)");
        default:
            return std::string("unknown");
    }
}

}
