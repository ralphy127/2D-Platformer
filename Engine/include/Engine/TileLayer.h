#pragma once

#include "Engine/Layer.h"
#include "Engine/TexturesManager.h"
#include "Engine/Camera.h"
#include "Engine/IMapTextures.h"
#include "Engine/ITileClassifier.h"

namespace engine {

/// @brief Represents a tile-based layer in the map.
class TileLayer : public ILayer, public ISettingsObserver {
public:
    using Grid = std::vector<std::vector<int>>;

    /// @brief Types of tile layers.
    enum class Type { FRONT_DECORATION, MAP, BACK_DECORATION };

    /// @brief Constructs a TileLayer.
    /// @param mapTextures Reference to the map textures manager.
    /// @param type Type of the tile layer.
    /// @param level Level index to load data from.
    /// @param tileSize Size of each tile in pixels.
    TileLayer(IMapTextures&, const ITileClassifier&, Settings&, Type, size_t level);

    ~TileLayer() { _settings.unregisterObserver(*this); }

    /// @brief Renders the tile layer.
    /// @param renderer SDL renderer used for drawing.
    /// @param camera Camera used for coordinate transformation.
    void render(SDL_Renderer&, Camera&) const override;

    /// @brief Gets the tile grid data.
    /// @return 2D grid of tile IDs.
    const Grid& getGridView() const;

    void onSettingsChanged() override { _tileSize = _settings.getTileSize(); }

private:
    /// @brief Gets the CSV path for tile data.
    /// @return Path to the tile data CSV file.
    std::string getCSVPath() const;

    /// @brief Loads tile data from a CSV file.
    /// @param path Path to the CSV file.
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
