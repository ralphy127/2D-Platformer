#include "Engine/Level.h"

namespace engine {

Level::Level(
    IMapTextures& mapTextures,
    const ITileClassifier& tileClassifier,
    Settings& settings,
    size_t level) 
    : _level(level) {
        
    try {
        const size_t layers = 1;
        _layers.resize(layers);
        _layers[0] = std::make_unique<TileLayer>(
                         mapTextures,
                         tileClassifier,
                         settings,
                         TileLayer::Type::MAP,
                         level);

        SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Level %zu created", _level);
    }
    catch(const std::runtime_error& e) {
        throw std::runtime_error("Loading level " + std::to_string(level) + " error: " + e.what());
    }
}

void Level::render(SDL_Renderer& renderer, Camera& camera) const {
    for(const auto& layer : _layers)
        layer->render(renderer, camera);
}

const TileLayer::Grid& Level::getMapView() const {
    if (auto* mapLayer = dynamic_cast<TileLayer*>(_layers[0].get())) {
        return mapLayer->getGridView();
    }
    throw std::runtime_error("Level " + std::to_string(_level) + " map not initialized");
}

}
