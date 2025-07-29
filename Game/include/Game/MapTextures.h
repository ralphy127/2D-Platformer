#pragma once

#include <vector>
#include <array>

#include <Engine/TexturesManager.h>
#include <Engine/IMapTextures.h>
#include <Utils/SDLUtils.h>
#include <Utils/Logging.h>

namespace game {

/// @brief Manages loading and access to map textures categorized by type (GROUND, BUILDING).
class MapTextures : public engine::IMapTextures {
public:
    /// @brief Constructor initializing the map textures with the given renderer
    /// @param renderer Pointer to SDL_Renderer used to create textures
    MapTextures(SDL_Renderer&);

    ~MapTextures() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Map textures destroyed"); }

    MapTextures(const MapTextures&) = delete;
    MapTextures& operator=(const MapTextures&) = delete;
    MapTextures(MapTextures&&) = delete;
    MapTextures& operator=(MapTextures&&) = delete;

    /// @brief Map texture types
    enum class TextureType { NONE, GROUND, BUILDING };
    
    /// @brief Retrieves a texture of the specified type and ID
    /// @param type Texture type (GROUND or BUILDING)
    /// @param id Texture identifier
    /// @return Pointer to SDL_Texture corresponding to the given type and id
    SDL_Texture& getTexture(int type, size_t id) override { return *_textures.at(type).at(id); }

private:
    SDL_Renderer& _renderer; ///< SDL renderer used for creating textures

    /// @brief Container holding textures grouped by type
    std::array<std::vector<utils::SDLUtils::TexturePtr>, 2> _textures{};
};

}
