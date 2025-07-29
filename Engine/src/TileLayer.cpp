#include "Engine/TileLayer.h"

#include <fstream>
#include <sstream>

namespace engine {

TileLayer::TileLayer(
    IMapTextures& mapTextures,
    const ITileClassifier& tileClassifier,
    Settings& settings,
    Type type,
    size_t level) 
    : _mapTextures(mapTextures),
      _tileClassifier(tileClassifier),
      _settings(settings),
      _type(type),
      _level(level) {

    _tileSize = _settings.getTileSize();
    auto path = getCSVPath();
    loadFromCSV(path);

    _settings.registerObserver(*this);
}

void TileLayer::render(SDL_Renderer& renderer, Camera& camera) const {
    auto rows = _grid.size();
    for (size_t row = 0; row < rows; ++row) {
        auto cols = _grid[row].size();
        for (size_t col = 0; col < cols; ++col) {
            auto tileId = _grid[row][col];

            auto [tileType, textureIndex] = _tileClassifier.classify(tileId);
            if (textureIndex == -1)
                continue;

            auto& tileTexture = _mapTextures.getTexture(tileType, textureIndex);

            utils::f2v pos(col * _tileSize, row * _tileSize);
            utils::f2v size(_tileSize, _tileSize);
            auto tileRect = camera.worldToViewport(pos, size);

            auto viewPort = camera.getViewport();


            if (!SDL_HasIntersection(&tileRect, &viewPort))
                continue;

            SDL_RenderCopy(&renderer, &tileTexture, nullptr, &tileRect);
        }
    }
}

const TileLayer::Grid& TileLayer::getGridView() const {
    return _grid;
}

std::string TileLayer::getCSVPath() const {
    std::string fileName;
    switch(_type) {
        case Type::MAP:
            fileName = "map";
            break;
        case Type::INTERACTIVE:
            fileName = "interactive";
            break;
        case Type::DECORATION:
            fileName = "decoration";
            break;
        default:
            throw std::runtime_error("failed to get csv path: wrong map type.");
    }

    return std::string("assets/levels/level_") + 
                      (_level < 10 ? "0" : "") + (_level < 100 ? "0" : "") 
                      + std::to_string(_level) + "/" + fileName + ".csv";
}

void TileLayer::loadFromCSV(const std::string& path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("failed to open tile map: " + path);

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string tileId;
        std::vector<int> row;

        while (std::getline(ss, tileId, ','))
            row.push_back(std::stoi(tileId));

        _grid.push_back(std::move(row));
    }
}

}
