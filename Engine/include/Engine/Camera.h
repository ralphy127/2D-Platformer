#pragma once

#include <SDL2/SDL.h>
#include <Utils/Logging.h>
#include <Utils/f2v.h>
#include <Utils/i2v.h>
#include "Engine/Settings.h"

namespace engine {

/// @brief Camera class for managing viewport and transformations.
class Camera : public ISettingsObserver {
public:
    /// @brief Constructs a Camera with position, zoom factor and reference to settings.
    /// @param pos Initial position of the camera in world coordinates.
    /// @param zoom Initial zoom factor.
    /// @param settings Reference to settings used for observing window size changes.
    Camera(Settings&);

    /// @brief Destructor. Unregisters from settings observer.
    ~Camera();
    
    /// @brief Gets the current viewport rectangle of the camera.
    /// @return SDL_Rect representing the visible area in world space.
    SDL_Rect getViewport() const;

    /// @brief Applies camera transformations to a given position and size.
    /// @param pos Position of the object in world coordinates.
    /// @param size Size of the object.
    /// @return SDL_Rect representing the transformed rectangle in screen coordinates.
    SDL_Rect worldToViewport(const utils::f2v& pos, const utils::f2v& size) const;

    /// @brief Centers the camera on a target position.
    /// @param target The position in world space to center the camera on.
    void centerOn(const utils::f2v& target);

    /// @brief Sets the zoom factor of the camera.
    /// @param zoom New zoom factor. Clamped internally to reasonable bounds.
    void setZoom(float zoom);

    /// @brief Sets the window size of the camera viewport.
    /// @param windowSize New window size in pixels.
    void setWindowSize(const utils::i2v& windowSize) { _windowSize = windowSize; }

    /// @brief Responds to settings change (e.g. window resized).
    void onSettingsChanged() override { setWindowSize(_settings.getWindowSize()); }

private:
    Settings& _settings;       ///< Reference to global settings.

    utils::i2v _pos{};         ///< Current top-left position of the camera in world space.
    utils::i2v _windowSize{};  ///< Size of the viewport window in pixels.
    float _zoom{};             ///< Zoom factor applied to camera projection.
};

}
