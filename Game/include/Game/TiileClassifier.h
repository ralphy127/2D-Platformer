#pragma once

#include <utility>
#include "Engine/ITileClassifier.h"
#include "Game/MapTextures.h"

namespace game {

class TileClassifier : public engine::ITileClassifier {
public:
    std::pair<int, int> classify(int tileId) const override;
};

}
