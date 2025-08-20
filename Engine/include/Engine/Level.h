#pragma once

#include <Utils/Logging.h>
#include "Engine/TileLayer.h"
#include "Engine/Settings.h"

namespace engine {

/// @brief Represents a game level composed of multiple layers.
class Level {
public:
    Level(IMapTextures&, const ITileClassifier&, Settings&, SDL_Renderer&, size_t level);
    ~Level() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Level %zu destroyed", _level); }

    /// @brief Renders all layers of the level.
    void render(SDL_Renderer&, Camera&) const;

    const TileLayer::Grid& getMapView() const;

private:
    size_t _level;                                  ///< id of the level
    std::vector<std::unique_ptr<ILayer>> _layers{}; ///< List of layers in the level.
};

}
