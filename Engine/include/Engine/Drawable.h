#pragma once

#include "IRenderable.h"
#include "Engine/Settings.h"
#include "Engine/Entity.h"

namespace engine {

enum class Direction : uint8_t {
    None,
    Left,
    Right
};

/// @brief Abstract class representing renderable entity
class Drawable : public Entity, public IRenderable {
public:
    /// @brief Configuration struct used to initialize a Drawable.
    struct Config : Entity::Config {
        utils::f2v textureSize; ///< Size of the texture used for rendering.
        Direction direction;    ///< Rendering direction (negative for left, positive for right).
    };

    Drawable(Settings& settings, const Config& config);

protected:
    Direction getDirection() const { return _direction; }
    void setDirection(Direction direction) { _direction = direction; }

    int8_t getDirectionMultiplicator() const;
    
    /// @brief Changes direction to opposite
    void turn();

    void onSettingsChanged() override;

protected:
    void setTextureSize(utils::f2v textureSize) { _textureSize = textureSize; }
    utils::f2v getTextureSize() const { return _textureSize; }

private:
    utils::f2v _textureSize; ///< Size of the associated texture.
    Direction _direction;    ///< Direction for rendering (e.g. sprite flipping).
};

}
