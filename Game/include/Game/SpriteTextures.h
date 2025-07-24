#pragma once

#include <unordered_map>
#include <vector>
#include <Engine/ISpriteTextures.h>
#include <Utils/SDLUtils.h>
#include <Utils/i2v.h>
#include <Utils/f2v.h>
#include <Utils/Logging.h>
#include <Game/EntityTypes.h>

namespace game {

struct TextureDefinition {
    std::vector<int> frameCounts;
    utils::i2v frameSize;
    utils::i2v margin;
};

class SpriteTextures : public engine::ISpriteTextures {
public:
    SpriteTextures(SDL_Renderer& renderer);
    
    ~SpriteTextures() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Sprite textures destroyed"); }

    SpriteTextures(const SpriteTextures&) = delete;
    SpriteTextures& operator=(const SpriteTextures&) = delete;
    SpriteTextures(SpriteTextures&&) = delete;
    SpriteTextures& operator=(SpriteTextures&&) = delete;

    SDL_Texture& getTexture(engine::Entity::Type, size_t animation, size_t frame) override;

private:
    void loadDefaultSprites();

    void loadSprite(EntityType);

    void defineSprites();

    SDL_Renderer& _renderer;

    std::unordered_map<EntityType, std::vector<std::vector<utils::SDLUtils::TexturePtr>>> _cache{};
    std::unordered_map<EntityType, TextureDefinition> _spriteDefinitions{};
};

}
