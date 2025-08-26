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

/// @brief Defines data for a sprite sheet's animations and frames.
struct TextureDefinition {
    std::vector<int> frameCounts; ///< Number of frames per animation.
    utils::i2v frameSize;         ///< Size (width, height) of each frame in pixels.
    utils::i2v margin;            ///< Margin between frames in the sprite sheet.
};

/// @brief Manages loading, storing, and retrieving animated sprite textures.
class SpriteTextures : public engine::ISpriteTextures {
public:
    SpriteTextures(SDL_Renderer& renderer);
    ~SpriteTextures() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Sprite textures destroyed"); }
    
    SDL_Texture& getTexture(engine::Entity::Type type, size_t animation, size_t frame) override;

private:
    /// @brief Loads all default sprite sheets and slices them into frames.
    void loadDefaultSprites();

    /// @brief Loads and processes sprite sheet for a specific entity type.
    void loadSprite(EntityType type);

    /// @brief Defines frame counts, sizes, and margins for each sprite sheet.
    void defineSprites();

    SDL_Renderer& _renderer; ///< Renderer used for texture creation.

    /// @brief Cache mapping entity types to their animations and frames textures.
    std::unordered_map<EntityType, std::vector<std::vector<utils::SDLUtils::TexturePtr>>> _cache{};
    /// @brief Data definitions for each sprite sheet.
    std::unordered_map<EntityType, TextureDefinition> _spriteDefinitions{};
};

}
