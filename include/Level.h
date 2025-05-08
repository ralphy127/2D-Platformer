#pragma once

#include "Camera.h"
#include "Layer.h"
#include "Utils.h"

#include <SDL2/SDL.h>

class Level {
public:
    Level(SDL_Renderer* renderer, unsigned int level, unsigned int tileSize, i2v& windowSize);
    ~Level();

    void load(int level);
    void render(SDL_Renderer* renderer, Camera& camera);

    std::vector<std::vector<int>>& getMapGrid() { return _layers[5].getGrid(); }

private:
    SDL_Renderer* _renderer;
    unsigned int _level;
    std::vector<Layer> _layers;
};