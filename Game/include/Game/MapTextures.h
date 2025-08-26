#pragma once

#include <vector>
#include <array>

#include <Engine/TexturesManager.h>
#include <Engine/IMapTextures.h>
#include <Utils/SDLUtils.h>
#include <Utils/Logging.h>

namespace game {

class MapTextures : public engine::IMapTextures {
public:
    MapTextures(SDL_Renderer&);
    ~MapTextures() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Map textures destroyed"); }

    enum class TextureType { GROUND, BUILDING, COUNT, NONE };
    
    SDL_Texture& getTexture(int type, size_t id) override { return *_textures.at(type).at(id); }

private:
    SDL_Renderer& _renderer; ///< SDL renderer used for creating textures

    /// @brief Container holding textures grouped by type
    std::array<std::vector<utils::SDLUtils::TexturePtr>,
               static_cast<size_t>(TextureType::COUNT)> _textures{};
};

}
