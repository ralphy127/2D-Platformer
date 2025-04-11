#include "Entity.h"

#include "Constants.h"
#include "TextureManager.h"
#include "Camera.h"

#include <iostream>

Entity::Entity(SDL_Renderer* renderer, f2v& pos, f2v& size, f2v& textureSize, float animationTargetTime, const std::string& fileName, int direction, const std::vector<int>& textureCounts,  i2v& singleSize, i2v& margin) :
    _renderer(renderer),
    _pos(pos),
    _size(size),
    _textureSize(textureSize),
    _lastDirection(direction),
    _animationTargetTime(animationTargetTime) {

    loadTextures(fileName, textureCounts, singleSize, margin);

    _lastAnimationTime = SDL_GetTicks();
    _currentAnimation = 0;
    _currentFrame = 0;
}

Entity::~Entity() {
    for(std::vector<SDL_Texture*> state_textures : _textures) {
        for(SDL_Texture* frame_texture : state_textures) {
            SDL_DestroyTexture(frame_texture);
        }
    }
}  

void Entity::loadTextures(const std::string& fileName, const std::vector<int>& textureCounts, const i2v& singleSize, const i2v& margin) {
    i2v size = { singleSize.x - 2 * margin.x, singleSize.y - margin.y};

    int n = textureCounts.size();

    for(int i = 0; i < n; i++) {
        std::vector<SDL_Texture*> stateTextures;
        for(int j = 0; j < textureCounts[i]; j++) {
            i2v pos = { j * singleSize.x + margin.x, i * singleSize.y + margin.y };
            stateTextures.push_back(TextureManager::loadCutTexture(_renderer, fileName, pos, size));
        }
        _textures.push_back(stateTextures);
    }
}

void Entity::render(Camera* camera) {
    f2v cameraPos(_pos.x - _size.x/2.f, _pos.y - _size.y/2.f);
    camera->centerOn(cameraPos);

    SDL_Texture* currentTexture = _textures[_currentAnimation][_currentFrame];
    if(!currentTexture) {
        std::cerr << "Error: no texture of entity animation: animation: " << _currentAnimation << " , frame: " << _currentFrame << '.' << std::endl;
        return;
    }

    SDL_RendererFlip flip = _lastDirection > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    f2v texturePos(_pos.x - (_textureSize.x - _size.x) / 2., _pos.y - (_textureSize.y - _size.y));

    SDL_Rect textureRect = camera->apply(texturePos, _textureSize);

    if(SHOW_HITBOXES) {
        SDL_Rect entityRect = camera->apply(_pos, _size);
        SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(_renderer, &entityRect);

        SDL_Rect textureRect = camera->apply(texturePos, _textureSize);
        SDL_SetRenderDrawColor(_renderer, 128, 128, 128, 255);
        SDL_RenderDrawRect(_renderer, &textureRect);
        
        if(_weaponHitboxPos.x != 0.f) {
            SDL_Rect entityWeaponRect = camera->apply(_weaponHitboxPos, _weaponHitboxSize);
            SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);
            SDL_RenderDrawRect(_renderer, &entityWeaponRect);
        }

        if(_protectingHitboxPos.x != 0.f) {
            SDL_Rect entityProtectingRect = camera->apply(_protectingHitboxPos, _protectingHitboxSize);
            SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
            SDL_RenderDrawRect(_renderer, &entityProtectingRect);
        }
    }

    if(SDL_RenderCopyEx(_renderer, currentTexture, nullptr, &textureRect, 0., nullptr, flip) < 0) {
        std::cerr << "SDL_RenderCopyEx Error: " << SDL_GetError() << std::endl;
    }
} 