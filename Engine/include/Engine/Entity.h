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
        utils::f2v pos; ///< Initial position of the entity.
        utils::f2v size;///< Size of the entity.
        Type type;      ///< Entity type identifier.
    };

    Entity(Settings&, const Config& config);
    ~Entity() { _settings.unregisterObserver(*this); }

    utils::f2v getPos() const { return _pos; }
    void setPos(utils::f2v pos) { _pos = pos; }

    utils::f2v getSize() const { return _size; }
    void setSize(utils::f2v size) { _size = size; }

    SDL_FRect getHitbox() const;
    void setHitbox(SDL_Rect hitbox);

protected:
    Type getType() const { return _type; }

    size_t getTileSize() const { return _tileSize; }

    const Settings& getSettings() const { return _settings; }

private:
    Settings& _settings; ///< Reference to shared engine settings.
    size_t _tileSize;    ///< Cached tile size.
    utils::f2v _pos;     ///< Current position of the entity.
    utils::f2v _size;    ///< Current size of the entity.
    Type _type;          ///< Identifier for the type of entity.
};

}
