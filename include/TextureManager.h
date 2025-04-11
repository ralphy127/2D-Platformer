#pragma once 

#include "Utils.h"

#include <SDL2/SDL.h>
#include <vector>
#include <string>

class TextureManager {
public:
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& fileName);
    static SDL_Texture* loadCutTexture(SDL_Renderer* renderer, const std::string& fileName, const i2v& pos, const i2v& size);
    static std::vector<SDL_Texture*> loadTexturesByName(SDL_Renderer* renderer, const std::string& baseName, const unsigned int count);
};