#include "Game/SimpleTextures.h"

#include <Engine/TexturesManager.h>

namespace game {

SimpleTextures::SimpleTextures(SDL_Renderer& renderer) 
    : _renderer(renderer) {

    loadDefaultTextures();

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Simple textures created");
}

SDL_Texture& SimpleTextures::getTexture(engine::Entity::Type typeId) {
    const auto type = static_cast<EntityType>(typeId);
    const auto it = _cache.find(type);
    if (it == _cache.end())
        throw std::runtime_error("Texture not found for simple entity type: " + toString(type));

    return *it->second;
}

void SimpleTextures::loadDefaultTextures() {
    for (auto type : allSimpleTypes)
        loadTexture(type);
}

void SimpleTextures::loadTexture(EntityType type) {
    auto path = "assets/textures/simple/" + toString(type) + ".png";
    _cache.emplace(type, engine::TexturesManager::loadFromFile(_renderer, path));
}

}
