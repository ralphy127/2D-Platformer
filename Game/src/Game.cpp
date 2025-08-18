#include "Game/Game.h"

#include "Game/Player.h"
#include "Game/Commander.h"
#include "Game/Archer.h"

namespace game {

Game::Game()
    : _settings(createDefaultSettingsConfig()),
      _windowManager(_settings),
      _physicsHandler{_settings},
      _clock(_settings), 
      _renderManager(_windowManager.getWindow()),
      _camera(_settings),
      _mapTextures(_renderManager.getRenderer()),
      _simpleTextures(_renderManager.getRenderer()),
      _spriteTextures(_renderManager.getRenderer()) {

    const auto windowSize = _windowManager.getWindowSize();
    _settings.setWindowSize(windowSize);
    
    const auto tileSize = static_cast<size_t>(windowSize.y / 40.f);
    _settings.setTileSize(tileSize);

    _physicsHandler.setGAcceleration(20.f * tileSize);
    _physicsHandler.setFallGMultiplier(1.8f);

    _camera.setZoom(2.f);

    size_t levels = 1;
    for (size_t level = 0; level < levels; ++level)
        _levels.push_back(std::make_unique<engine::Level>(_mapTextures,
            _tileClassifier,
            _settings,
            _renderManager.getRenderer(),
            level));

    _entities.push_back(std::make_unique<Commander>(_settings, _spriteTextures));
    _entities.push_back(std::make_unique<Archer>(_settings, _spriteTextures));

    _entities.push_back(std::make_unique<Player>(_settings, _spriteTextures, _eventHandler));

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
    const auto deltaTime = _clock.getDeltaTime();

    try {
        for (auto& entity : _entities) {
            if (auto* dynamicEntity = dynamic_cast<engine::DynamicEntity*>(entity.get())) {
                dynamicEntity->update(deltaTime);
                _physicsHandler.applyGravity(*dynamicEntity, deltaTime);
                _physicsHandler.handleMapCollisions(
                    *dynamicEntity,
                    _levels[_currentLevel]->getMapView(),
                    deltaTime);

                dynamicEntity->applyMovement(deltaTime);
            }
        }

        auto& player = dynamic_cast<Player&>(*_entities.back());
        player.logDebugState();
    }
    catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Game update error: %s", e.what());
    }
}

void Game::render() {
    _renderManager.clear();

    auto& renderer = _renderManager.getRenderer();

    try {
        _levels[_currentLevel]->render(renderer, _camera);

        for (const auto& entity : _entities) {
            if (auto* renderable = dynamic_cast<engine::IRenderable*>(entity.get())) {
                renderable->render(renderer, _camera);
            }
        }
    }
    catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Game render error: %s", e.what());
    }

    _renderManager.present();
}

}
