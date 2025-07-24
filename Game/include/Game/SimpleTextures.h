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

    SimpleTextures(const SimpleTextures&) = delete;
    SimpleTextures(SimpleTextures&&) = delete;
    SimpleTextures& operator=(const SimpleTextures&) = delete;
    SimpleTextures& operator=(SimpleTextures&&) = delete;

    SDL_Texture& getTexture(engine::Entity::Type) override;

private:
    void loadDefaultTextures();

    void loadTexture(EntityType);

    SDL_Renderer& _renderer;

    std::unordered_map<EntityType, utils::SDLUtils::TexturePtr> _cache{};
};

}
