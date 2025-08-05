#pragma once

#include <unordered_map>
#include <Engine/ISimpleTextures.h>
#include <Utils/SDLUtils.h>
#include <Utils/Logging.h>
#include <Game/EntityTypes.h>

namespace game {

/// @brief Manages and provides access to simple entity textures.
class SimpleTextures : public engine::ISimpleTextures {
public:
    /// @brief Constructs the SimpleTextures manager and loads default textures.
    /// @param renderer SDL_Renderer used for texture creation.
    SimpleTextures(SDL_Renderer&);

    ~SimpleTextures() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Simple textures created"); }

    /// @brief Retrieves the SDL texture associated with the given entity type.
    /// @param type The entity type to get the texture for.
    /// @return Reference to the cached SDL_Texture for the entity type.
    SDL_Texture& getTexture(engine::Entity::Type) override;

private:
    /// @brief Loads all default textures required for simple entities.
    void loadDefaultTextures();

    /// @brief Loads a texture for a specific entity type and stores it in the cache.
    /// @param type The entity type for which to load the texture.
    void loadTexture(EntityType);

    SDL_Renderer& _renderer; ///< Renderer used for texture creation.

    /// @brief Cache mapping entity types to their loaded textures.
    std::unordered_map<EntityType, utils::SDLUtils::TexturePtr> _cache{};
};

}
