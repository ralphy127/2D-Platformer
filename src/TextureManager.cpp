#include "TextureManager.h"

#include <iostream>
#include <sstream>

#include <SDL2/SDL_image.h>

SDL_Texture* TextureManager::loadTexture(SDL_Renderer* renderer, const std::string& fileName) {
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if(!tempSurface) {
        std::cerr << "IMG_Load error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if(!texture) {
        std::cerr << "SDLCreatureTextureFromSurface error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    return texture;
}

SDL_Texture* TextureManager::loadCutTexture(SDL_Renderer* renderer, const std::string& fileName, const i2v& pos, const i2v& size) {
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if(!tempSurface) {
        std::cerr << "IMG_Load error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* originalTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if(!originalTexture) {
        std::cerr << "SDLCreatureTextureFromSurface error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* cutTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
    if(!cutTexture) {
        std::cerr << "SDL_CreateTexture error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(originalTexture);
        return nullptr;
    }
    
    SDL_SetRenderTarget(renderer, cutTexture);
    SDL_Rect srcRect = { pos.x, pos.y, size.x, size.y };
    SDL_Rect destRect = { 0, 0, size.x, size.y };
    SDL_RenderCopy(renderer, originalTexture, &srcRect, &destRect);

    SDL_SetRenderTarget(renderer, nullptr);

    SDL_DestroyTexture(originalTexture);

    SDL_SetTextureBlendMode(cutTexture, SDL_BLENDMODE_BLEND);

    return cutTexture;
}

std::vector<SDL_Texture*> TextureManager::loadTexturesByName(SDL_Renderer* renderer, const std::string& baseName, const unsigned int count) {
    std::vector<SDL_Texture*> textures;

    for(int i = 1; i <= count; i++) {
        std::stringstream ss;
        ss << baseName << (i < 10 ? "0" : "") << i << ".png";
        const std::string& fileName = ss.str();

        SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
        if(!tempSurface) {
            std::cerr << "IMG_Load error: " << IMG_GetError() << std::endl;
            textures.push_back(nullptr);
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        if(!texture) {
            std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
            textures.push_back(nullptr);
            continue;
        }

        textures.push_back(texture);
    }

    return textures;
}