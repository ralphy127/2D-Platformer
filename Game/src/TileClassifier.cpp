#include "Game/TiileClassifier.h"

namespace game {

std::pair<int, int> TileClassifier::classify(int tileId) const {
    if (tileId >= 0 && tileId < 13)
        return { 0, tileId }; // ground
    if (tileId >= 100 && tileId < 161)
        return { 1, tileId }; // building

    return { -1, -1 };
}

}
