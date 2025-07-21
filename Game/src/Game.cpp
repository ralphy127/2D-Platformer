#include "Game/Game.h"

namespace game {

Game::Game()
    : _windowManager(_settings),
      _clock(_settings),
      _renderManager(_windowManager.getWindow()),
      _camera(utils::i2v(0,0), 2.f, _settings),
      _mapTextures(_renderManager.getRenderer()),
      _simpleTextures(_renderManager.getRenderer()),
      _spriteTextures(_renderManager.getRenderer()),
      _player(nullptr)
{        
    auto windowSize = _windowManager.getWindowSize();
    _settings.setWindowSize(windowSize);
    
    auto tileSize = static_cast<size_t>(windowSize.y / 40.f);
    _settings.setTileSize(tileSize);

    size_t levels = 1;
    for(size_t level = 0; level < levels; ++level)
        _levels.push_back(engine::Level(_mapTextures, _tileClassifier, _settings, level));

    _player = std::make_unique<Player>(_settings, _spriteTextures);

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Game created");
}

void Game::run() {
    while(_running) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    _eventHandler.pollEvents();

    if(_eventHandler.isQuitRequested() || _eventHandler.isKeyPressed(SDLK_q))
        _running = false;
}

void Game::update() { 
    _clock.tickAndWait();
    auto deltaTime = _clock.getDeltaTime();

    _player->update(deltaTime, _eventHandler.getKeyStates());
}

void Game::render() {
    _renderManager.clear();

    auto& renderer = _renderManager.getRenderer();

    for(const auto& level : _levels)
        level.render(renderer, _camera);

    for(const auto& entity : _dynamicEntities)
        entity->render(renderer, _camera);

    _player->render(renderer, _camera);

    _renderManager.present();
}

}
