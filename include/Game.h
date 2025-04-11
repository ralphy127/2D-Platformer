#pragma once

#include "Utils.h"
#include "Camera.h"
#include "Player.h"
#include "Level.h"

#include <SDL2/SDL.h>

#include <vector>
#include <unordered_map>

class Game {
public:
    Game();
    ~Game();

    void handleEvents();
    void update();
    void render();

    bool isRunning() { return _running; }
    float getLastFrameTime() { return _lastFrameTime; }

private:
    bool initSDL();

private:
    bool _running;
    bool _fullscreen;
    unsigned int _masterVolume;
    i2v _windowSize;
    unsigned int _tileSize;

    SDL_Window* _window;
    SDL_Renderer* _renderer;

    float _lastFrameTime;

    std::unordered_map<SDL_Keycode, bool> _keyStates;

    Camera* _camera;
    std::unique_ptr<Player> _player;

    unsigned int _currentLevel;
    std::vector<Level> _levels;
}; 