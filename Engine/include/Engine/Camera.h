#pragma once

#include <SDL2/SDL.h>
#include <Utils/Logging.h>
#include <Utils/f2v.h>
#include <Utils/i2v.h>
#include "Engine/Settings.h"

namespace engine {

/// @brief Camera class for managing viewport and transformations.
class Camera : ISettingsObserver {
public:
    /// @brief Constructs a Camera with position, window size and zoom factor.
    /// @param pos Initial position of the camera.
    /// @param windowSize Size of the window viewport.
    /// @param zoom Zoom factor for the camera.
    Camera(utils::i2v pos, float zoom, Settings&);

    ~Camera();

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;
    
    /// @brief Gets the current viewport rectangle of the camera.
    /// @return SDL_Rect representing the viewport.
    SDL_Rect getViewport() const;

    /// @brief Applies camera transformations to a given position and size.
    /// @param pos Position of the object in world coordinates.
    /// @param size Size of the object.
    /// @return SDL_Rect representing the transformed rectangle in screen coordinates.
    SDL_Rect worldToViewport(const utils::f2v& pos, const utils::f2v& size) const;

    /// @brief Centers the camera on a target position.
    /// @param target The position to center the camera on.
    void centerOn(const utils::f2v& target);

    /// @brief Sets the zoom factor of the camera.
    /// @param zoom New zoom factor.
    void setZoom(float zoom);

    /// @brief Sets the window size of the camera viewport.
    /// @param windowSize New window size.
    void setWindowSize(const utils::i2v& windowSize) { _windowSize = windowSize; }

    void onSettingsChanged() override { setWindowSize(_settings.getWindowSize()); }

private:
    Settings& _settings;

    utils::i2v _pos;          ///< Current position of the camera.
    utils::i2v _windowSize{}; ///< Size of the viewport window.
    float _zoom{};     ///< Zoom factor applied to transformations.

    static constexpr float MAX_ZOOM = 100.0f; ///< Maximum value of zoom
};

}
