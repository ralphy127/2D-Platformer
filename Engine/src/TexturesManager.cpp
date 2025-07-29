#include "Engine/TexturesManager.h"

#include <SDL_image.h>

namespace engine {

utils::SDLUtils::TexturePtr TexturesManager::loadFromFile(
    SDL_Renderer& renderer,
    const std::string& fileName) {
        
    auto surface = utils::SDLUtils::makeSurface(IMG_Load(fileName.c_str()));
    if (!surface)
        throw std::runtime_error("Failed to create surface: " + std::string(IMG_GetError()));

    auto texture = utils::SDLUtils::makeTexture(
        SDL_CreateTextureFromSurface(&renderer, surface.get()));

    if(!texture) 
        throw std::runtime_error("Failed to load texture: " + std::string(IMG_GetError()));

    return texture;
}

utils::SDLUtils::TexturePtr TexturesManager::loadClippedFromFile(
    SDL_Renderer& renderer,
    const std::string& fileName,
    const utils::i2v& pos,
    const utils::i2v& size) { 

    auto originalTexture = loadFromFile(renderer, fileName);

    auto cutTexture = utils::SDLUtils::makeTexture(
        SDL_CreateTexture(
            &renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            size.x,
            size.y));

    if(!cutTexture)
        throw std::runtime_error("Failed to cut texture: " + std::string(IMG_GetError()));

    auto prevTarget = SDL_GetRenderTarget(&renderer);
    SDL_SetRenderTarget(&renderer, cutTexture.get());

    SDL_Rect srcRect = { pos.x, pos.y, size.x, size.y };
    SDL_Rect destRect = { 0, 0, size.x, size.y };
    SDL_RenderCopy(&renderer, originalTexture.get(), &srcRect, &destRect);

    SDL_SetRenderTarget(&renderer, nullptr);
    SDL_SetTextureBlendMode(cutTexture.get(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(&renderer, prevTarget);

    return cutTexture;
}

std::vector<utils::SDLUtils::TexturePtr> TexturesManager::loadSequenceFromPattern(
    SDL_Renderer& renderer,
    const std::string& pattern,
    const size_t count,
    std::string extension) {

    std::vector<utils::SDLUtils::TexturePtr> textures;
    textures.reserve(count);

    for(size_t i = 0; i < count; ++i) {
        auto fileName = pattern + (i < 10 ? "0" : "") + std::to_string(i) + extension;
        textures.push_back(loadFromFile(renderer, fileName));
    }

    return textures;
}

}
