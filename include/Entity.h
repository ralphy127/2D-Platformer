#pragma once

#include "Utils.h"
#include "Camera.h"

#include <SDL2/SDL.h>
#include <vector>
#include <string>

class Entity {
public:
    Entity(SDL_Renderer* renderer, f2v& pos, f2v& size, f2v& textureSize, float animationTargetTime, const std::string& fileName, int direction, const std::vector<int>& textureCounts,  i2v& singleSize, i2v& margin);
    ~Entity();

    void render(Camera* camera);

private:
    void loadTextures(const std::string& fileName, const std::vector<int>& textureCounts, const i2v& singleSize, const i2v& margin);

protected:
    SDL_Renderer* _renderer;

    f2v _pos;
    f2v _size;
    f2v _textureSize;

    std::vector<std::vector<SDL_Texture*> > _textures;

    int _lastDirection;
    float _lastAnimationTime;
    float _animationTargetTime;
    unsigned int _currentAnimation;
    unsigned int _currentFrame;

    f2v _weaponHitboxPos;
    f2v _weaponHitboxSize;
    f2v _protectingHitboxPos;
    f2v _protectingHitboxSize;
};