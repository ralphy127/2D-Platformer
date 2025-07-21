#include "Game/MapTextures.h"

namespace game {

MapTextures::MapTextures(SDL_Renderer& renderer) : _renderer(renderer) {
    _textures[0] = engine::TexturesManager::loadSequenceFromPattern(_renderer, "assets/textures/Village/Platformer/Ground_", 13);
    
    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Map textures created");
}

}