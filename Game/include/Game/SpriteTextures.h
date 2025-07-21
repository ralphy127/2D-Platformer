#pragma once

#include <unordered_map>
#include <vector>
#include <Engine/EntityTypes.h>
#include <Engine/ISpriteTextures.h>
#include <Utils/SDLUtils.h>
#include <Utils/i2v.h>
#include <Utils/f2v.h>
#include <Utils/Logging.h>

namespace game {

struct TextureDefinition {
    std::vector<int> frameCounts;
    utils::i2v frameSize;
    utils::i2v margin;
};

class SpriteTextures : public engine::ISpriteTextures {
public:
    using Type = engine::EntityTypes::Sprite;

    SpriteTextures(SDL_Renderer& renderer);
    
    ~SpriteTextures() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Sprite textures destroyed"); }

    SpriteTextures(const SpriteTextures&) = delete;
    SpriteTextures& operator=(const SpriteTextures&) = delete;
    SpriteTextures(SpriteTextures&&) = delete;
    SpriteTextures& operator=(SpriteTextures&&) = delete;

    SDL_Texture& getTexture(Type, size_t animation, size_t frame) override;

private:
    void loadDefaultSprites();

    void loadSprite(engine::EntityTypes::Sprite);

    void defineSprites();

    SDL_Renderer& _renderer;

    std::unordered_map<Type, std::vector<std::vector<utils::SDLUtils::TexturePtr>>> _cache{};
    std::unordered_map<Type, TextureDefinition> _spriteDefinitions{};
};

}
