#include "Game/MapTextures.h"

namespace game {

MapTextures::MapTextures(SDL_Renderer& renderer) : _renderer(renderer) {
    engine::MapTextureType groundType = 0;
    uint32_t groundCount = 13;
    _textures.at(groundType) = engine::TexturesManager::loadSequenceFromPattern(
        _renderer,
        "assets/textures/Village/Platformer/Ground_",
        groundCount);
    
    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Map textures created");
}

}