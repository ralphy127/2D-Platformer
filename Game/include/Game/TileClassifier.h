#pragma once

#include <utility>
#include "Engine/ITileClassifier.h"
#include "Game/MapTextures.h"

namespace game {

class TileClassifier : public engine::ITileClassifier {
public:
    /// @brief Classifies tile ID into type and texture index.
    /// @param tileId The tile identifier from the map data.
    /// @return Pair of (tileType, textureIndex) for collision and rendering.
    std::pair<int, int> classify(int tileId) const override;
};

}
