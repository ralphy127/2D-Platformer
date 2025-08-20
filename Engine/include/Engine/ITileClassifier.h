#pragma once

namespace engine {

/// @brief Interface for tile classification provider.
class ITileClassifier {
public:
    virtual ~ITileClassifier() = default;

    /// @brief Classifies a tile ID into type and texture index.
    /// @param tileId The tile id.
    /// @return Pair of (tileType, textureIndex) for collision and rendering.
    virtual std::pair<int, int> classify(int tileId) const = 0;
};

}
