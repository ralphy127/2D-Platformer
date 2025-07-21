#pragma once

#include <unordered_map>
#include <Engine/EntityTypes.h>
#include <Engine/ISimpleTextures.h>
#include <Utils/SDLUtils.h>
#include <Utils/Logging.h>

namespace game {

class SimpleTextures : public engine::ISimpleTextures {
public:
    using Type = engine::EntityTypes::Simple;

    SimpleTextures(SDL_Renderer&);

    ~SimpleTextures() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Simple textures created"); }

    SimpleTextures(const SimpleTextures&) = delete;
    SimpleTextures(SimpleTextures&&) = delete;
    SimpleTextures& operator=(const SimpleTextures&) = delete;
    SimpleTextures& operator=(SimpleTextures&&) = delete;

    SDL_Texture& getTexture(Type) override;

private:
    void loadDefaultTextures();

    void loadTexture(Type);

    SDL_Renderer& _renderer;

    std::unordered_map<Type, utils::SDLUtils::TexturePtr> _cache{};
};

}
