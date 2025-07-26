#pragma once

#include <Utils/f2v.h>
#include "Engine/Settings.h"

namespace engine {

/// @brief Base class representing any object in the world with a position and size.
class Entity : public ISettingsObserver {
public:
    using Type = int;

    /// @brief Configuration structure used for constructing entities.
    struct Config {
        utils::f2v pos;   ///< Initial position of the entity.
        utils::f2v size;  ///< Size of the entity.
        Type type;        ///< Entity type identifier.
    };

    /// @brief Constructs an Entity with the given configuration.
    /// @param settings Reference to engine settings.
    /// @param config Entity configuration (position, size, type).
    Entity(Settings&, const Config& config);

    ~Entity() { _settings.unregisterObserver(*this); }

    /// @brief Called when settings are changed. Updates cached tile size.
    void onSettingsChanged() override;

    /// @brief Gets the position of the entity.
    /// @return Current position.
    utils::f2v getPos() const { return _pos; }

    /// @brief Sets the position of the entity.
    /// @param pos New position to set.
    void setPos(utils::f2v pos) { _pos = pos; }

    /// @brief Gets the size of the entity.
    /// @return Current size.
    utils::f2v getSize() const { return _size; }

    /// @brief Sets the size of the entity.
    /// @param size New size to set.
    void setSize(utils::f2v size) { _size = size; }

    SDL_Rect getHitBox() const;

    void setHitBox(SDL_Rect hitbox);

protected:
    /// @brief Returns the type of the entity.
    /// @return Integer identifier of the entity type.
    Type getType() const { return _type; }

    /// @brief Returns current tile size from settings.
    /// @return Size of a single tile in pixels.
    size_t getTileSize() const { return _tileSize; }

    /// @brief Provides access to settings object.
    /// @return Const reference to the settings.
    const Settings& getSettings() const { return _settings; }

private:
    Settings& _settings; ///< Reference to shared engine settings.
    size_t _tileSize;    ///< Cached tile size (updated on settings change).
    utils::f2v _pos;     ///< Current position of the entity.
    utils::f2v _size;    ///< Current size of the entity.
    Type _type;          ///< Identifier for the type of entity.
};

}
