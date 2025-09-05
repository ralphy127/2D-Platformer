#pragma once

namespace engine {

using TileId = int16_t;
using TileType = int16_t;

struct TileClassification {
    TileId id;
    TileType type;
};

/// @brief Interface for tile classification provider.
class ITileClassifier {
public:
    virtual ~ITileClassifier() = default;

    /// @brief Classifies a tile ID into type and texture index.
    /// @return Pair of (tileType, textureIndex) for collision and rendering.
    virtual TileClassification classify(TileId) const = 0;
};

}
