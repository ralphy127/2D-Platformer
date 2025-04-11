#pragma once

#include "Utils.h"
#include "Entity.h"

#include <SDL2/SDL.h>
#include <vector>
#include <unordered_map>

class Player : public Entity {
public:
    Player(SDL_Renderer* renderer, f2v& pos, f2v& size, f2v& textureSize, unsigned int animationTargetTime, const std::string& fileName, int direction, const std::vector<int>& textureCounts,  i2v& singleSize, i2v& margin, float defaultSpeed);

    void update(float deltaTime, std::unordered_map<SDL_Keycode, bool> keyStates);

    void setMapGrid(const std::vector<std::vector<int>>& mapGrid) { _mapGrid = mapGrid; }; // do usuniecia z atrybutami
    void setTileSize(const int& tileSize) { _tileSize = tileSize; } // do usuniecia z atrybutami

public:
enum State { IDLE, WALKING, RUNNING, ATTACK1, ATTACK2, ATTACK3, PROTECTING, JUMPING, HURT, DEAD };

private:

private:
    f2v _vel;
    float _defaultSpeed;

    float _health;

    State _state;

    std::vector<std::vector<int>> _mapGrid;
    int _tileSize;

    bool _isOnGround;

    float _jumpVel;
};