#include "Game/SpriteTextures.h"

#include <Engine/TexturesManager.h>

namespace game {

SpriteTextures::SpriteTextures(SDL_Renderer& renderer)
    : _renderer(renderer) {

    defineSprites();
    loadDefaultSprites();

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Sprite textures created");
}

SDL_Texture& SpriteTextures::getTexture(engine::Entity::Type typeId, size_t animation, size_t frame) {
    const auto type = static_cast<EntityType>(typeId);
    const auto it = _cache.find(type);
    if (it == _cache.end())
        throw std::runtime_error("Textures not found for sprite entity type: " + toString(type));

    auto& textures = it->second;

    try {
        return *textures.at(animation).at(frame);
    } 
    catch (const std::out_of_range&) {
        throw std::runtime_error(
            std::string("Sprite texture not found - Type: ") + toString(type) +
            " Anim: " + std::to_string(animation) + 
            " Frame: " + std::to_string(frame)
        );
    }
}

void SpriteTextures::loadDefaultSprites() {
    for (auto type : allSpriteTypes) 
        loadSprite(type);
}

void SpriteTextures::loadSprite(EntityType type) {
    if(_cache.count(type) > 0) return;

    auto& definition = _spriteDefinitions.at(type);

    auto& frameCounts = definition.frameCounts;
    auto frameSize = definition.frameSize;
    auto margin = definition.margin;

    utils::i2v size = { frameSize.x - 2 * margin.x, frameSize.y - margin.y};
    int n = frameCounts.size();

    auto path = "assets/textures/sprites/" + toString(type) + ".png";

    std::vector<std::vector<utils::SDLUtils::TexturePtr>> sprites;
    for (int i = 0; i < n; ++i) {
        std::vector<utils::SDLUtils::TexturePtr> stateTextures;
        auto frames = frameCounts[i];
        for(int j = 0; j < frames; ++j) {
            utils::i2v pos = { j * frameSize.x + margin.x, i * frameSize.y + margin.y };
            auto texture = engine::TexturesManager::loadClippedFromFile(_renderer, path, pos, size);
            stateTextures.push_back(std::move(texture));
        }
        sprites.push_back(std::move(stateTextures));
    }

    _cache.emplace(type, std::move(sprites));
}

void SpriteTextures::defineSprites() {
    TextureDefinition playerDef {
        {6, 9, 8, 4, 5, 4, 2, 9, 3, 6},
        utils::i2v(128, 128),
        utils::i2v(0, 0)
    };
    _spriteDefinitions[EntityType::PLAYER] = std::move(playerDef);
};

}
