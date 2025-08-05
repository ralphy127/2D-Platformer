#include "Engine/Camera.h"

namespace engine {

Camera::Camera(Settings& settings)
    : _settings(settings) {

    _windowSize = _settings.getWindowSize();

    _settings.registerObserver(*this);

    SDL_LogDebug(utils::LOG_CATEGORY_SETUP, "Camera created");
}

Camera::~Camera() {
    _settings.unregisterObserver(*this);

    SDL_LogDebug(utils::LOG_CATEGORY_CLEANUP, "Camera destroyed");
}

SDL_Rect Camera::getViewport() const {
    return {
        0,
        0,
        static_cast<int>(_windowSize.x),
        static_cast<int>(_windowSize.y)
    };
}

SDL_Rect Camera::worldToViewport(const utils::f2v& pos, const utils::f2v& size) const {
    return {
        static_cast<int>((pos.x - _pos.x) * _zoom),
        static_cast<int>((pos.y - _pos.y) * _zoom),
        static_cast<int>(size.x * _zoom),
        static_cast<int>(size.y * _zoom)
    };
}

void Camera::centerOn(const utils::f2v& target) {
    _pos = {
        static_cast<int>(target.x - (_windowSize.x / 2.f) / _zoom),
        static_cast<int>(target.y - (_windowSize.y / 2.f) / _zoom)
    };
}

void Camera::setZoom(float zoom) { 
    if (zoom <= 0.0f) 
        throw std::invalid_argument("Zoom must be positive");

    _zoom = zoom;
}

}
