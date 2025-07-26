#pragma once

#include <Utils/Logging.h>
#include "Engine/TileLayer.h"
#include "Engine/Settings.h"

namespace engine {

/// @brief Represents a game level composed of multiple tile layers.
class Level {
public:
    /// @brief Constructs a level with specified textures, settings, and level number.
    /// @param mapTextures Reference to the map textures.
    /// @param settings Game settings.
    /// @param level Level index to load.
    Level(IMapTextures&, const ITileClassifier&, Settings&, size_t level);

    ~Level() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Level %zu destroyed", _level); }

    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;
    Level(Level&&) = default;
    Level& operator=(Level&&) = default;

    /// @brief Renders all layers of the level.
    /// @param renderer SDL renderer.
    /// @param camera Camera used to transform tile positions.
    void render(SDL_Renderer&, Camera&) const;

    const TileLayer::Grid& getMapView() const;

private:
    size_t _level; ///< id of the level

    std::vector<std::unique_ptr<Layer>> _layers{}; ///< List of layers in the level.
};

}
