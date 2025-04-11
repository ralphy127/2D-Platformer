#include "MapTextures.h"
#include "TextureManager.h"

#include <iostream>

MapTextures::~MapTextures() {
    for(SDL_Texture* texture : _groundTextures) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void MapTextures::init(SDL_Renderer* renderer) {
    if(_initialized) {
        return;
    }
    if(!renderer) {
        std::cerr << "Error: renderer is null in MapTextures" << std::endl;
        return;
    }

    _renderer = renderer;
    loadTextures();
    _initialized = true;
}

void MapTextures::loadTextures() {
    _groundTextures = TextureManager::loadTexturesByName(_renderer, "../assets/textures/Village/Platformer/Ground_", 13);
    _villageBuildingTextures = TextureManager::loadTexturesByName(_renderer, "../assets/textures/Village/Building/Building_", 61);
}

SDL_Texture* MapTextures::getTexture(TextureType type, int k) {
    switch(type) {
        case GROUND:
            if(k < 0 || k > _groundTextures.size()) {
                std::cerr << "Error: wrong map texture value." << std::endl;
                return nullptr;
            }
            return _groundTextures[k-1];
        case BUILDING:
            if(k < 101 || k > _villageBuildingTextures.size() + 100) {
                std::cerr << "Error: wrong map texture value." << std::endl;
                return nullptr;
            }
            return _villageBuildingTextures[k-101];
        default:
            std::cerr << "Error: wrong map texture type." << std::endl;
            return nullptr;
    }
    return nullptr;
}