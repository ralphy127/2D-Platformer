#pragma once

#include <list>
#include <Engine/SDLManager.h>
#include <Engine/WindowManager.h>
#include <Engine/EventHandler.h>
#include <Engine/PhysicsHandler.h>
#include <Engine/Clock.h>
#include <Engine/RenderManager.h>
#include <Engine/Camera.h>
#include <Engine/Level.h>
#include "Game/MapTextures.h"
#include "Game/TileClassifier.h"
#include "Game/SimpleTextures.h"
#include "Game/SpriteTextures.h"
#include "Game/Player.h"

namespace game {

class Game {
public:
    Game();
    ~Game() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Game destroyed"); }

    void run();

private:
    static engine::Settings::Config createDefaultSettingsConfig();

    void initLevels();
    void initEntities();

    void handleEvents();

    void update();
    void updateEntity(engine::DynamicEntity&, const engine::TileLayer::Grid&, float deltaTime);

    void render();
 
    engine::Settings _settings;             ///< Stores configuration settings.
    engine::SDLManager _SDLManager{};       ///< Initializes and shuts down SDL.
    engine::WindowManager _windowManager;   ///< Manages the SDL window and renderer.
    engine::EventHandler _eventHandler{};   ///< Handles SDL events.
    engine::PhysicsHandler _physicsHandler; ///< Manages physics calculations.
    engine::Clock _clock;                   ///< Manages frame timing and delta time.
    engine::RenderManager _renderManager;   ///< Clears and presents the renderer.
    engine::Camera _camera;                 ///< Controls the view into the game world.
    MapTextures _mapTextures;               ///< Manages map-related textures.
    TileClassifier _tileClassifier{};       ///< Classifies tiles (e.g., collision types).
    SimpleTextures _simpleTextures;         ///< Manages textures for simple entities.
    SpriteTextures _spriteTextures;         ///< Manages textures for sprite entities.

    bool _running{true};                    ///< Indicates if the game loop is active.

    engine::LevelId _currentLevel{0UL};     ///< Active level index
    std::vector<std::unique_ptr<engine::Level>> _levels{}; ///< Stores loaded levels.

    std::unique_ptr<Player> player{};       ///< Entity controlled by player
    std::list<std::unique_ptr<engine::Entity>> _entities{}; ///< Holds all entities in the game.

};

}
