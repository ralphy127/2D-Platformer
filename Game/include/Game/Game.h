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
#include "Game/TiileClassifier.h"
#include "Game/SimpleTextures.h"
#include "Game/SpriteTextures.h"
#include "Game/Player.h"
#include "Engine/StaticEntity.h"

namespace game {

/// @brief Core class managing the main game loop and essential subsystems.
class Game {
public:
    /// @brief Constructs the Game object and initializes all subsystems.
    Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    ~Game() { SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Game destroyed"); }

    /// @brief Starts and runs the main game loop.
    void run();

private:
    /// @brief Creates a default configuration for game settings.
    /// @return A configured Settings::Config struct with default values.
    static engine::Settings::Config createDefaultSettingsConfig();

    /// @brief Handles user and system events.
    void handleEvents();

    /// @brief Updates the game state.
    void update();

    /// @brief Renders the current game frame.
    void render();

    engine::Settings _settings;                 ///< Stores configuration settings.
    engine::SDLManager _SDLManager{};           ///< Initializes and shuts down SDL.
    engine::WindowManager _windowManager;       ///< Manages the SDL window and renderer.
    engine::EventHandler _eventHandler{};       ///< Handles SDL events.
    engine::PhysicsHandler _physicsHandler;     ///< Manages physics calculations.
    engine::Clock _clock;                       ///< Manages frame timing and delta time.
    engine::RenderManager _renderManager;       ///< Responsible for clearing and presenting the renderer.
    engine::Camera _camera;                     ///< Controls the view into the game world.
    MapTextures _mapTextures;                   ///< Manages map-related textures.
    TileClassifier _tileClassifier{};           ///< Classifies tiles (e.g., collision types).
    SimpleTextures _simpleTextures;             ///< Manages textures for simple entities.
    SpriteTextures _spriteTextures;             ///< Manages textures for sprite entities.

    std::vector<engine::Level> _levels{};       ///< Stores loaded levels.

    bool _running{true};                        ///< Indicates if the game loop is active.

    std::list<std::unique_ptr<engine::Entity>> _entities{}; ///< Holds all entities in the game.
};

}
