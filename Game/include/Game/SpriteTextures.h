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

/// @brief Defines metadata for a sprite sheet's animations and frames.

struct TextureDefinition {
    std::vector<int> frameCounts;  ///< Number of frames per animation.
    utils::i2v frameSize;          ///< Size (width, height) of each frame in pixels.
    utils::i2v margin;             ///< Margin between frames in the sprite sheet.
};

/// @brief Manages loading, storing, and retrieving animated sprite textures.
class SpriteTextures : public engine::ISpriteTextures {
public:
    /// @brief Constructs the SpriteTextures manager and loads default sprite sets.
    /// @param renderer SDL_Renderer used to create textures.
    SpriteTextures(SDL_Renderer& renderer);
    
    ~SpriteTextures() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Sprite textures destroyed"); }

    SpriteTextures(const SpriteTextures&) = delete;
    SpriteTextures& operator=(const SpriteTextures&) = delete;
    SpriteTextures(SpriteTextures&&) = delete;
    SpriteTextures& operator=(SpriteTextures&&) = delete;

    /// @brief Retrieves the texture for a specific entity type, animation, and frame.
    /// @param type Entity type identifying the sprite.
    /// @param animation Animation index (0-based).
    /// @param frame Frame index within the animation (0-based).
    /// @return Reference to the SDL_Texture representing the requested frame.
    SDL_Texture& getTexture(engine::Entity::Type type, size_t animation, size_t frame) override;

private:
    /// @brief Loads all default sprite sheets and slices them into frames.
    void loadDefaultSprites();

    /// @brief Loads and processes sprite sheet for a specific entity type.
    /// @param type EntityType to load sprite textures for.
    void loadSprite(EntityType type);

    /// @brief Defines frame counts, sizes, and margins for each sprite sheet.
    void defineSprites();

    SDL_Renderer& _renderer; ///< Renderer used for texture creation.

    /// @brief Cache mapping entity types to their animations and frames textures.
    std::unordered_map<EntityType, std::vector<std::vector<utils::SDLUtils::TexturePtr>>> _cache{};

    /// @brief Metadata definitions for each sprite sheet.
    std::unordered_map<EntityType, TextureDefinition> _spriteDefinitions{};
};

} // namespace game
