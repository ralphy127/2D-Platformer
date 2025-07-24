#include "Game/Game.h"

namespace game {

Game::Game()
    : _settings(createDefaultSettingsConfig()),
      _windowManager(_settings),
      _clock(_settings),
      _renderManager(_windowManager.getWindow()),
      _camera(utils::i2v(0,0), 2.f, _settings),
      _mapTextures(_renderManager.getRenderer()),
      _simpleTextures(_renderManager.getRenderer()),
      _spriteTextures(_renderManager.getRenderer()) {
                
    auto windowSize = _windowManager.getWindowSize();
    _settings.setWindowSize(windowSize);
    
    auto tileSize = static_cast<size_t>(windowSize.y / 40.f);
    _settings.setTileSize(tileSize);

    size_t levels = 1;
    for (size_t level = 0; level < levels; ++level)
        _levels.push_back(engine::Level(_mapTextures, _tileClassifier, _settings, level));

    auto player = std::make_unique<Player>(_settings, _spriteTextures, _eventHandler);
    _entities.push_back(std::move(player));

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Game created");
}

void Game::run() {
    while (_running) {
        handleEvents();
        update();
        render();
    }
}

engine::Settings::Config Game::createDefaultSettingsConfig() {
    engine::Settings::Config config{};
    config.fullscreen = true;
    config.title = "2D Platformer";
    config.targetFps = 60.f;
    config.showHitboxes = true;
    config.showTextureHitboxes = true;
    config.windowSize = utils::i2v(720, 460);
    config.windowPos = utils::i2v(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    config.tileSize = 0UL;

    return config;
}

void Game::handleEvents() {
    _eventHandler.pollEvents();

    if (_eventHandler.isQuitRequested() || _eventHandler.isKeyPressed(SDLK_q))
        _running = false;
}

void Game::update() { 
    _clock.tickAndWait();
    auto deltaTime = _clock.getDeltaTime();

    for (auto& entity : _entities) {
        if (auto* updatable = dynamic_cast<engine::IUpdatable*>(entity.get())) {
            updatable->update(deltaTime);
        }
    }
}

void Game::render() {
    _renderManager.clear();

    auto& renderer = _renderManager.getRenderer();

    for (const auto& level : _levels)
        level.render(renderer, _camera);

    for (const auto& entity : _entities) {
        if (auto* renderable = dynamic_cast<engine::IRenderable*>(entity.get())) {
            renderable->render(renderer, _camera);
        }
    }

    _renderManager.present();
}

}
