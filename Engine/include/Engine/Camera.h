#pragma once

#include <SDL2/SDL.h>
#include <Utils/Logging.h>
#include <Utils/f2v.h>
#include <Utils/i2v.h>
#include "Engine/Settings.h"

namespace engine {

class Camera : public ISettingsObserver {
public:
    Camera(Settings&);
    ~Camera();
    
    /// @brief Gets the current viewport rectangle of the camera.
    SDL_Rect getViewport() const;

    /// @brief Applies camera transformations to a given position and size.
    /// @param pos Position of the object in world coordinates.
    /// @param size Size of the object.
    /// @return SDL_Rect representing the transformed rectangle in screen coordinates.
    SDL_Rect worldToViewport(const utils::f2v& pos, const utils::f2v& size) const;

    /// @brief Centers the camera on a target position.
    void centerOn(const utils::f2v& target);

    float32_t getZoom() const { return _zoom; }
    void setZoom(float32_t zoom);

    void setWindowSize(const utils::i2v& windowSize) { _windowSize = windowSize; }

    void onSettingsChanged() override { setWindowSize(_settings.getWindowSize()); }

private:
    Settings& _settings;       ///< Reference to global settings.

    utils::i2v _pos{};         ///< Current top-left position of the camera in world space.
    utils::i2v _windowSize{};  ///< Size of the viewport window in pixels.
    float32_t _zoom{};         ///< Zoom factor applied to camera projection.
};

}
