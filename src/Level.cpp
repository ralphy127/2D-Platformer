#include "Level.h"

#include <iostream>

Level::Level(SDL_Renderer* renderer, unsigned int level, unsigned int tileSize, i2v& windowSize) : _renderer(renderer), _level(level) {
    if(!_renderer) {
        std::cerr << "Error: renderer is null in level: " << level << '.' << std::endl;
    }

    _layers.push_back(Layer(renderer, Layer::BACKGROUND, level, 1));
    _layers.push_back(Layer(renderer, Layer::FAR, level, 1));
    _layers.push_back(Layer(renderer, Layer::NEAR, level, 1));
    _layers.push_back(Layer(Layer::DECORATION, level, tileSize, windowSize));
    _layers.push_back(Layer(Layer::INTERACTIVE, level, tileSize, windowSize));
    _layers.push_back(Layer(Layer::MAP, level, tileSize, windowSize));
}

Level::~Level() {
    
}

void Level::render(SDL_Renderer* renderer, Camera& camera) {
    for(Layer& layer : _layers) {
        layer.render(renderer, camera);
    }
}