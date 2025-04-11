#pragma once

#include <SDL2/SDL.h>

#include "Utils.h"

class Camera {
public:
    Camera(i2v& pos, i2v& windowSize, float zoom);

    SDL_Rect getViewport();
    SDL_Rect apply(const f2v& pos, const f2v& size);
    void centerOn(const f2v& target);

private:
    i2v _pos;
    i2v _windowSize;
    float _zoom;
};