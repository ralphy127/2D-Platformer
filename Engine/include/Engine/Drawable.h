#pragma once

#include "IRenderable.h"
#include "Engine/Settings.h"
#include "Engine/Entity.h"

namespace engine {

/// @brief Abstract base class representing a drawable entity with position, size and texture data.
class Drawable : public Entity, public IRenderable {
public:
    /// @brief Configuration struct for initializing a Drawable.
    struct Config : Entity::Config {
        utils::f2v textureSize; ///< Size of the texture used for rendering.
        int direction;          ///< Rendering direction (-1 for left, 1 for right).
    };

    /// @brief Constructs a Drawable entity with the given settings and configuration.
    /// @param settings Reference to the Settings object.
    /// @param config Configuration data for the drawable entity.
    Drawable(Settings& settings, const Config& config);

    /// @brief Callback triggered when settings change (e.g. tile size, window size).
    void onSettingsChanged() override;

    /// @brief Gets the rendering direction.
    /// @return Integer representing direction (-1 or 1).
    int getDirection() const { return _direction; }

    /// @brief Sets the rendering direction.
    /// @param direction New direction value.
    void setDirection(int direction) { _direction = direction; }

    void turn() { _direction *= -1; }

protected:
    /// @brief Gets the texture size.
    /// @return Texture size as a vector.
    utils::f2v getTextureSize() const { return _textureSize; }

    /// @brief Sets the texture size.
    /// @param textureSize New texture size.
    void setTextureSize(utils::f2v textureSize) { _textureSize = textureSize; }

private:
    utils::f2v _textureSize; ///< Size of the associated texture.
    int _direction;          ///< Direction for rendering (e.g. sprite flipping).
};

}
