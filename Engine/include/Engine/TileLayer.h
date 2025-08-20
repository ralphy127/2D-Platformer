#pragma once

#include "Engine/ILayer.h"
#include "Engine/TexturesManager.h"
#include "Engine/Camera.h"
#include "Engine/IMapTextures.h"
#include "Engine/ITileClassifier.h"

namespace engine {

/// @brief Represents a tile-based layer in the map.
class TileLayer : public ILayer, public ISettingsObserver {
public:
    /// @brief Map grid data
    using Grid = std::vector<std::vector<int>>;

    /// @brief Types of tile layers.
    enum class Type { BACK_DECORATION = 3, MAP, FRONT_DECORATION};

    TileLayer(IMapTextures&, const ITileClassifier&, Settings&, Type, size_t level);
    ~TileLayer() { _settings.unregisterObserver(*this); }

    void render(SDL_Renderer&, Camera&) const override;

    const Grid& getGridView() const;

    void onSettingsChanged() override { _tileSize = _settings.getTileSize(); }

private:
    /// @brief Gets the CSV path for tile data.
    std::string getCSVPath() const;

    /// @brief Loads tile data from a CSV file.
    void loadFromCSV(const std::string& path);
    
    IMapTextures& _mapTextures;             ///< Reference to the map textures manager.
    const ITileClassifier& _tileClassifier; /// Reference to tileClassifier
    Settings& _settings;                    ///< Reference to game settings
    Type _type;                             ///< Type of the tile layer.
    size_t _levelId;                        ///< Level index.
    size_t _tileSize;                       ///< Size of each tile.
    Grid _grid;                             ///< Grid of tile IDs.
};


}
