#include "Game/TileClassifier.h"

namespace game {

engine::TileClassification TileClassifier::classify(engine::TileId id) const {
    if (id >= 0 && id < 13)
        return { 0, id }; // ground
    if (id >= 100 && id < 161)
        return { 1, id }; // building

    return { -1, -1 };
}

}
