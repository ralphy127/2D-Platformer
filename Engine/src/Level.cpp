#include "Engine/Level.h"

#include "Engine/ParallaxLayer.h"

namespace engine {

Level::Level(
    IMapTextures& mapTextures,
    const ITileClassifier& tileClassifier,
    Settings& settings,
    SDL_Renderer& renderer,
    LevelId id) 
    : _id(id) {
        
    try {
        _layers.at(0) = std::make_unique<ParallaxLayer>(
            settings,
            renderer,
            ParallaxLayer::Type::BACKGROUND,
            _id,
            0.f);
        _layers.at(1) = std::make_unique<ParallaxLayer>(
            settings,
            renderer,
            ParallaxLayer::Type::FAR,
            _id,
            0.f);
        _layers.at(4) = std::make_unique<TileLayer>(
            mapTextures,
            tileClassifier,
            settings,
            TileLayer::Type::MAP,
            _id);

        // TODO 2, 3, 5 initialization

        SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Level %hu created", _id);
    }
    catch(const std::runtime_error& e) {
        throw std::runtime_error("Loading level " + std::to_string(_id) + " error: " + e.what());
    }
}

void Level::render(SDL_Renderer& renderer, Camera& camera) const {
    for(const auto& layer : _layers) 
        if (layer) layer->render(renderer, camera);
}

const TileLayer::Grid& Level::getMapView() const {
    if (auto* mapLayer = dynamic_cast<TileLayer*>(
            _layers.at(static_cast<size_t>(TileLayer::Type::MAP)).get())) {

        return mapLayer->getGridView();
    }
    throw std::runtime_error("Level " + std::to_string(_id) + " map not initialized");
}

}
