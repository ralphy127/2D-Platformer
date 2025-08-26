#pragma once

#include <unordered_map>
#include <Engine/ISimpleTextures.h>
#include <Utils/SDLUtils.h>
#include <Utils/Logging.h>
#include <Game/EntityTypes.h>

namespace game {

class SimpleTextures : public engine::ISimpleTextures {
public:
    SimpleTextures(SDL_Renderer&);
    ~SimpleTextures() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Simple textures created"); }

    SDL_Texture& getTexture(engine::Entity::Type) override;

private:
    /// @brief Loads all default textures required for simple entities.
    void loadDefaultTextures();

    /// @brief Loads a texture for a specific entity type and stores it in the cache.
    void loadTexture(EntityType);

    SDL_Renderer& _renderer; ///< Renderer used for texture creation.
    
    /// @brief Cache mapping entity types to their loaded textures.
    std::unordered_map<EntityType, utils::SDLUtils::TexturePtr> _cache{};
};

}
