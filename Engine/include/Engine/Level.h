#pragma once

#include <Utils/Logging.h>
#include "Engine/TileLayer.h"
#include "Engine/Settings.h"

namespace engine {

/// @brief Represents a game level composed of multiple layers.
class Level {
public:
    Level(IMapTextures&, const ITileClassifier&, Settings&, SDL_Renderer&, LevelId);
    ~Level() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Level %hu destroyed", _id); }

    /// @brief Renders all layers of the level.
    void render(SDL_Renderer&, Camera&) const;

    const TileLayer::Grid& getMapView() const;

private:
    LevelId _id;                                    ///< id of the level
    std::vector<std::unique_ptr<ILayer>> _layers{}; ///< List of layers in the level.
};

}
