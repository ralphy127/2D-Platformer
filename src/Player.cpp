#include "Player.h"

#include "Physics.h"
#include "Constants.h"

#include <iostream>

Player::Player(SDL_Renderer* renderer, f2v& pos, f2v& size, f2v& textureSize, unsigned int animationTargetTime, const std::string& fileName, int direction, const std::vector<int>& textureCounts,  i2v& singleSize, i2v& margin, float defaultSpeed) 
    : Entity(renderer, pos, size, textureSize, animationTargetTime, fileName, direction, textureCounts, singleSize, margin) {

    _vel = { 0.f, 0.f };
    _defaultSpeed = defaultSpeed / 20;
    _health = 10;
    _state = IDLE;
    _isOnGround = false;
    _jumpVel = defaultSpeed / 4;
}

void Player::update(float deltaTime, std::unordered_map<SDL_Keycode, bool> keyStates) {
    const bool left  = keyStates[SDLK_LEFT];
    const bool right = keyStates[SDLK_RIGHT];
    const bool shift = keyStates[SDLK_LSHIFT];

    if(left && right) {
        _vel.x = _defaultSpeed * _lastDirection;
        _currentAnimation = WALKING;
    } 
    else if(left) {
        _vel.x = -_defaultSpeed * (shift ? 3 : 1);
        _lastDirection = -1;
        _currentAnimation = shift ? RUNNING : WALKING;
    } 
    else if(right) {
        _vel.x = _defaultSpeed * (shift ? 3 : 1);
        _lastDirection = 1;
        _currentAnimation = shift ? RUNNING : WALKING;
    } 
    else{
        _vel.x = 0;
        _currentAnimation = IDLE;
    }

    if (!_isOnGround) {
        if(_vel.y < 0) {
            _vel.y += G_ACC * deltaTime;
        } else {
            _vel.y += 1.8f * G_ACC * deltaTime;
        }
    } else {
        _vel.y = 0;
    }

    if(keyStates[SDLK_e]) {
        _currentAnimation = PROTECTING;
        if(_vel.x != 0) {
            _vel.x = _defaultSpeed * _lastDirection / 3.f;
        }
    }

    _isOnGround = false;

    SDL_Rect playerRect = {
        static_cast<int>(_pos.x),
        static_cast<int>(_pos.y),
        static_cast<int>(_size.x),
        static_cast<int>(_size.y)
    };

    bool collisionX = false;
    bool collisionY = false;
    int x = 0;
    int y = 0;

    for(const auto& row : _mapGrid) {
        int x = 0;
        for(const int& tile : row) {
            if(tile != 0) {
                SDL_Rect tileRect = {x, y, _tileSize, _tileSize};
                raycastHit hit;
                
                if(AABBcast(playerRect, tileRect, _vel, hit, deltaTime)) {
                    if (fabsf(hit.normal.x) > 0.5f) {
                        collisionX = true;
                    }
                    if (fabsf(hit.normal.y) > 0.5f) {
                        collisionY = true;
                        if(hit.normal.y < 0) {
                            _isOnGround = true;
                            // _pos.y = static_cast<int>(y - _size.y);
                            _pos.y = y - _size.y;
                            _vel.y = 0.f;
                        }
                    }
                }
            }

            x += _tileSize;
        }

        y += _tileSize;
    }
    
    if(!collisionX) {
        _pos.x += _vel.x * deltaTime;
    } else {
        _vel.x = 0;
    }

    if(!collisionY) {
        _pos.y += _vel.y * deltaTime;
    } else if(_vel.y > 0) {
        _isOnGround = true; 
    }

    bool attacking = false;
    if(keyStates[SDLK_z]) {
        _currentAnimation = ATTACK1;

        float offsetX = 0.6f * _size.x;
        _weaponHitboxPos = {
            _lastDirection == 1 ? _pos.x + offsetX : _pos.x + _size.x - offsetX - 1.75f * _size.x,
            _pos.y + 0.2f * _size.y };

        _weaponHitboxSize = {1.75f * _size.x, 0.45f * _size.y};

        attacking = true;
    } 
    else if(keyStates[SDLK_x]) {
        _currentAnimation = ATTACK2;

        float offsetX = 0.15f * _size.x;
        _weaponHitboxPos = {
            _lastDirection == 1 ? _pos.x + offsetX : _pos.x + _size.x - offsetX - 2.3f * _size.x,
            _pos.y - 0.45f * _size.y };

        _weaponHitboxSize = {2.3f * _size.x, 1.3f * _size.y};

        attacking = true;
    } 
    else if(keyStates[SDLK_c]) {
        _currentAnimation = ATTACK3;

        float offsetX = 0.1f * _size.x;
        _weaponHitboxPos = {
            _lastDirection == 1 ? _pos.x - offsetX : _pos.x + _size.x + offsetX - 2.1f * _size.x,
            _pos.y + 0.05f * _size.y };

        _weaponHitboxSize = {2.1f * _size.x, 0.65f * _size.y};
        attacking = true;
    }

    if(attacking && _vel.x != 0.f) {
        _vel.x = _defaultSpeed * _lastDirection / 2.f;
    } 
    else if(!attacking) {
        _weaponHitboxPos = { 0.f, 0.f};
    }

    if(keyStates[SDLK_UP] && _isOnGround) {
        _vel.y = -_jumpVel;
        _isOnGround = false;
    }  

    if(keyStates[SDLK_p]) {
        _currentAnimation = DEAD;
    } else if(keyStates[SDLK_o]) {
        _currentAnimation = HURT;
    }

    // if(!_isOnGround) {
    //     _currentAnimation = JUMPING;
    // }

    float time = SDL_GetTicks();
    if (time - _lastAnimationTime >= _animationTargetTime) {
        _lastAnimationTime = time;
        _currentFrame++;
    }

    if(_currentFrame >= _textures[_currentAnimation].size()) {
        _currentFrame = 0;
    }
}