#pragma once

#include <Utils/f2v.h>

#include "Engine/Settings.h"

namespace engine {

class Entity : public ISettingsObserver {
public:
    Entity(utils::f2v pos, utils::f2v size, Settings&);

    ~Entity() { _settings.unregisterObserver(*this); }

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

    virtual bool isUpdatable() const { return false; }
    virtual bool isRenderable() const { return false; }

protected:
    size_t getTileSize() const { return _tileSize; }
    const Settings& getSettings() const { return _settings; }

private:
    Settings& _settings;
    size_t _tileSize;
    utils::f2v _pos;
    utils::f2v _size;
};

}
