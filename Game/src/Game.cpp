#include "Game/Game.h"

#include <filesystem>
#include <set>
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
    
    const auto tileSize = static_cast<uint32_t>(windowSize.y / 40.f);
    _settings.setTileSize(tileSize);

    _physicsHandler.setGAcceleration(20.f * tileSize);
    _physicsHandler.setFallGMultiplier(1.8f);

    _camera.setZoom(2.f);

    initLevels();
    initEntities();

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Game created");
}

void Game::initLevels() {
    const std::string levelsPath{"assets/levels"};

    std::set<int> levelIds;

    for (const auto& entry : std::filesystem::directory_iterator(levelsPath)) {
        if (!entry.is_directory()) continue;
        const auto name = entry.path().filename().string();

        if (name.rfind("level_", 0) == 0) {
            try {
                const auto id = std::stoi(name.substr(6));
                levelIds.insert(id);
            }
            catch (const std::exception&) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Invalid level folder name: %s", name.c_str());
            }
        }
    }

    _levels.reserve(levelIds.size());
    for (const auto id : levelIds) {
        _levels.emplace_back(std::make_unique<engine::Level>(
            _mapTextures,
            _tileClassifier,
            _settings,
            _renderManager.getRenderer(),
            id
        ));
    }
}

void Game::initEntities() {
    player = std::make_unique<Player>(_settings, _spriteTextures, _eventHandler);

    _entities.push_back(std::make_unique<Commander>(_settings, _spriteTextures));
    _entities.push_back(std::make_unique<Archer>(_settings, _spriteTextures));
}

void Game::run() {
    while (_running) {
        handleEvents();
        update();
        render();
    }
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
        const auto& map = _levels.at(_currentLevel)->getMapView();

        updateEntity(*player, map, deltaTime);

        for (auto& entityPtr : _entities) {
            if (auto* dynEntity = dynamic_cast<engine::DynamicEntity*>(entityPtr.get())) {
                updateEntity(*dynEntity, map, deltaTime);
                _physicsHandler.handleAttacksCollisions(*player, *dynEntity);
            }
        }

        player->logDebugState();
    }
    catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Game update error: %s", e.what());
    }
}

void Game::updateEntity(engine::DynamicEntity& entity, const engine::TileLayer::Grid& map, float deltaTime) {
    entity.update(deltaTime);
    _physicsHandler.applyGravity(entity, deltaTime);
    _physicsHandler.handleMapCollisions(entity, map, deltaTime);
    entity.applyMovement(deltaTime);
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

        player->render(renderer, _camera);
    }
    catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Game render error: %s", e.what());
    }

    _renderManager.present();
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

}
