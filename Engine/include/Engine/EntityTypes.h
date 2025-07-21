#pragma once

#include <string> 
#include <array>

namespace engine {

namespace EntityTypes {
    enum class Simple { COUNT, NONE };

    inline std::array<Simple, static_cast<size_t>(Simple::COUNT)> allSimple = {
        // types
    };

    enum class Sprite { PLAYER, COUNT, NONE };

    inline std::array<Sprite, static_cast<size_t>(Sprite::COUNT)> allSprite = {
        Sprite::PLAYER,
    };
}

inline std::string toString(EntityTypes::Sprite type) {
    switch (type) {
        case EntityTypes::Sprite::PLAYER:
                return std::string("player");
        default:
            return std::string("unknown");
    }
}

inline std::string toString(EntityTypes::Simple type) {
    switch (type) {
        default:
            return std::string("unknown");
    }
}

}
