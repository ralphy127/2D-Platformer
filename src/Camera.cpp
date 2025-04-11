#include "Camera.h"

Camera::Camera(i2v& pos, i2v& windowSize, float zoom) : _pos(pos), _windowSize(windowSize), _zoom(zoom) {}

SDL_Rect Camera::getViewport() {
    return { 
        static_cast<int>(_pos.x), 
        static_cast<int>(_pos.y), 
        static_cast<int>(_windowSize.x / _zoom), 
        static_cast<int>(_windowSize.y / _zoom) };
}

SDL_Rect Camera::apply(const f2v& pos, const f2v& size) {
    return {
        static_cast<int>((pos.x - _pos.x) * _zoom),
        static_cast<int>((pos.y - _pos.y) * _zoom),
        static_cast<int>(size.x * _zoom),
        static_cast<int>(size.y * _zoom) };
}

void Camera::centerOn(const f2v& target) {
    _pos = { 
        static_cast<int>(target.x - (_windowSize.x / (2 * _zoom))), 
        static_cast<int>(target.y - (_windowSize.y / (2 * _zoom))) };
}