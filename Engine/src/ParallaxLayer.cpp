#include "Engine/ParallaxLayer.h"

namespace engine {

ParallaxLayer::ParallaxLayer(
    Settings& settings,
    SDL_Renderer& renderer,
    Type type,
    LevelId levelId,
    float32_t parallaxSpeed)
    : _settings(settings),
      _type(type),
      _levelId(levelId),
      _parallaxSpeed(parallaxSpeed) {

    _settings.registerObserver(*this);

    _tileSize = _settings.getTileSize();

    try {
        const auto path = getTexturePath();
        _texture = TexturesManager::loadFromFile(renderer, path);
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to initialize parallax texture of levelId") +
            std::to_string(_levelId) + ", error: " + e.what());
    }
}

std::string ParallaxLayer::getTexturePath() const {
    std::string fileName;
    switch (_type) {
        case Type::NEAR:
            fileName = "near";
            break;
        case Type::FAR:
            fileName = "far";
            break;
        case Type::BACKGROUND:
            fileName = "background";
            break;
        default:
            throw std::runtime_error("failed to get texture: wrong type");
    }

    return std::string("assets/levels/level_") +
        (_levelId < 10 ? "0" : "") + (_levelId < 100 ? "0" : "") +
        std::to_string(_levelId) + "/textures/" + fileName + ".png";
}

void ParallaxLayer::render(SDL_Renderer& renderer, Camera& camera) const {
    if (SDL_RenderCopy(&renderer, _texture.get(), nullptr, nullptr) < 0)
        throw std::runtime_error(
            std::string("Failed to render parallax texture of levelId: ") +
            std::to_string(_levelId));
}

void ParallaxLayer::onSettingsChanged() {
    const auto oldTileSize = _tileSize;
    _tileSize = _settings.getTileSize();

    const auto fraction = static_cast<float>(_tileSize) / oldTileSize;
    _parallaxSpeed *= fraction;
}

}