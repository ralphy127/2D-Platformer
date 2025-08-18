#include "Engine/Level.h"

#include "Engine/ParallaxLayer.h"

namespace engine {

Level::Level(
    IMapTextures& mapTextures,
    const ITileClassifier& tileClassifier,
    Settings& settings,
    SDL_Renderer& renderer,
    size_t level) 
    : _level(level) {
        
    try {
        _layers.push_back(std::make_unique<ParallaxLayer>(
            settings,
            renderer,
            ParallaxLayer::Type::BACKGROUND,
            level,
            0.f
        ));

        _layers.push_back(std::make_unique<ParallaxLayer>(
            settings,
            renderer,
            ParallaxLayer::Type::FAR,
            level,
            0.f
        ));

        _layers.push_back(nullptr);
        _layers.push_back(nullptr);

        _layers.push_back(std::make_unique<TileLayer>(
            mapTextures,
            tileClassifier,
            settings,
            TileLayer::Type::MAP,
            level
        ));

        _layers.push_back(nullptr);

        SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Level %zu created", _level);
    }
    catch(const std::runtime_error& e) {
        throw std::runtime_error("Loading level " + std::to_string(level) + " error: " + e.what());
    }
}

void Level::render(SDL_Renderer& renderer, Camera& camera) const {
    for(const auto& layer : _layers) 
        // TODO if to be removed
        if (layer)
            layer->render(renderer, camera);
}

const TileLayer::Grid& Level::getMapView() const {
    if (auto* mapLayer = dynamic_cast<TileLayer*>(
            _layers.at(static_cast<size_t>(TileLayer::Type::MAP)).get())) {

        return mapLayer->getGridView();
    }
    throw std::runtime_error("Level " + std::to_string(_level) + " map not initialized");
}

}
