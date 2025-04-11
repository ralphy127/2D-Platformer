#include "Game.h"
#include "Constants.h"
#include "MapTextures.h"

#include <iostream>
#include <string>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

unsigned int WINDOW_WIDTH, WINDOW_HEIGHT, TILE_SIZE;
float G_ACC;

Game::Game() :
    _running(false),
    _fullscreen(FULLSCREEN),
    _masterVolume(1),
    _window(nullptr),
    _renderer(nullptr),
    _camera(nullptr) {
    
    if(!initSDL()) {
        _running = false;
        return;
    }
    
    _lastFrameTime = SDL_GetTicks();

    MapTextures::getInstance().init(_renderer);
    _tileSize = static_cast<unsigned int>(0.1 * _windowSize.y);

    _currentLevel = 1;
    _levels.push_back(Level(_renderer, _currentLevel, _tileSize, _windowSize));

    f2v playerSize(0.85 * _tileSize, 1.8 * _tileSize);
    f2v playerPos(_windowSize.x * 0.5 - playerSize.x/2.,_windowSize.y * 0.5 - playerSize.y/2.f);
    f2v playerTextureSize(3 * _tileSize, 3 * _tileSize);
    float playerAnimationTargetTime = 1000/7;
    std::string playerFileName = "../assets/textures/Samurai/sprite_sheet.png";
    int playerDirection = 1;
    std::vector<int> playerTextureCounts = {6, 9, 8, 4, 5, 4, 2, 9, 3, 6};
    i2v playerSingleTextureSize(128, 128);
    i2v playerTextureMargin(0, 0);
    float playerDefaultSpeed = 50 * _tileSize;

    _player = std::make_unique<Player>(_renderer, playerPos, playerSize, playerTextureSize, playerAnimationTargetTime, playerFileName, playerDirection, playerTextureCounts, playerSingleTextureSize, playerTextureMargin, playerDefaultSpeed);
    _player->setMapGrid(_levels[_currentLevel-1].getMapGrid());
    _player->setTileSize(_tileSize);

    i2v cameraPos(static_cast<int>(playerPos.x - playerSize.x/2.f), static_cast<int>(playerPos.y - playerSize.y/2.f));
    float cameraZoom = 0.6f;

    _camera = new Camera(cameraPos, _windowSize, cameraZoom);

    G_ACC = _tileSize * 20;

    _running = true;
    std::cout << "\nGame running...\n" << std::endl;
}

Game::~Game() {
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    SDL_CloseAudio();
    SDL_Quit();

    delete _camera;

    std::cout << "Game cleaned!" << std::endl;
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_QUIT:
            _running = false;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_q) {
                _running = false;
            }
            _keyStates[event.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            _keyStates[event.key.keysym.sym] = false;
            break;
    }
}

void Game::update() {
    float timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - _lastFrameTime);
    if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - _lastFrameTime) / 1000.f;
    _lastFrameTime = SDL_GetTicks();

    _player->update(deltaTime, _keyStates);
}

void Game::render() {
    SDL_SetRenderDrawColor(_renderer, 50, 200, 200, 255);
    SDL_RenderClear(_renderer);

    _levels[_currentLevel-1].render(_camera);
    _player->render(_camera);

    SDL_RenderPresent(_renderer);
}

bool Game::initSDL() {
    int flags = 0;

    if(_fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;

        if(SDL_Init(SDL_INIT_VIDEO)) {
            std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_DisplayMode displayMode;
        if(SDL_GetCurrentDisplayMode(0, &displayMode)) {
            std::cerr << "SDL_GetCurrentDisplayMode error: " << SDL_GetError() << std::endl;
            return false;
        }

        if(displayMode.w <= 0 || displayMode.h <= 0) {
            std::cerr << "Error: failed to initialize window dimensions." << std::endl;
            return false;
        }

        _windowSize.x = displayMode.w;
        _windowSize.y = displayMode.h;
    }
    else {
        _windowSize.x = 720;
        _windowSize.y = 450;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr << "Error: failed to initialize SDL; " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "SDL initialized!" << std::endl;

    _window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowSize.x, _windowSize.y, flags);
    if (!_window) {
        std::cerr << "Error: failed to create SDL_Window; " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "Window created!" << std::endl;

    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if (!_renderer) {
        std::cerr << "Error: failed to create SDL_Renderer; " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "Renderer created!" << std::endl;

    if(TTF_Init() < 0) {
        std::cerr << "Error: failed to initialize SDL fonts; " << SDL_GetError() << std::endl;
        return false;
    }

    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Error: failed to initialize SDL audio subsystem; " << SDL_GetError() << std::endl;
        return false;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Error: failed to initialize SDL mixer; " << Mix_GetError() << std::endl;
        return false;
    }

    Mix_MasterVolume(static_cast<int>(_masterVolume / 10. * SDL_MIX_MAXVOLUME));

    return true;
}