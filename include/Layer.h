#pragma once

#include "Camera.h"
#include "Utils.h"

#include <SDL2/SDL.h>

#include <vector>
#include <iostream>

class Layer {
public:
enum Type { BACKGROUND, FAR, NEAR, DECORATION, INTERACTIVE, MAP };

    Layer(SDL_Renderer* renderer, Type type, unsigned int level, int parallaxVel);
    Layer(SDL_Renderer* renderer, Type type, unsigned int level, unsigned int tileSize, i2v& windowSize);
    ~Layer();

    void render(Camera* camera);

    std::vector<std::vector<int>>& getGrid() { return _grid; }

private:
    SDL_Renderer* _renderer;
    Type _type;
    unsigned int _level;
    float _parallaxVel;
    
    SDL_Texture* _texture;
    std::vector<std::vector<int>> _grid;
    unsigned int _tileSize;
    i2v _windowSize;

    void renderNonGrid(Camera* camera);
    void renderGrid(Camera* camera);
};