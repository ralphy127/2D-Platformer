#include "Engine/SpriteData.h"

#include <stdexcept>
#include <chrono>
#include <iostream>

namespace engine {

SpriteData::SpriteData(AnimationsInfo animationsInfo, size_t initialAnimation) 
    : _animationsInfo(std::move(animationsInfo)),
      _currentAnimation(initialAnimation),
      _lastFrameTime(ClockType::now()) {
        
    if (_currentAnimation >= _animationsInfo.size()) {
        throw std::invalid_argument("Initial animation out of range");
    }

    fetchAnimationsInfo();
}

void SpriteData::fetchAnimationsInfo() {
    _framesInCurrentAnimation = _animationsInfo.at(_currentAnimation).first;
    _targetFrameDuration = _animationsInfo.at(_currentAnimation).second;
}

void SpriteData::setFrame(size_t frame) {
    if (frame >= _framesInCurrentAnimation)
        throw std::invalid_argument("Frames cannot be greater than frames in current animation");

    _currentFrame = frame;
    _lastFrameTime = ClockType::now();
}

void SpriteData::updateFrame() {
    auto now = ClockType::now();

    if (now - _lastFrameTime < _targetFrameDuration)
        return;

    _currentFrame = (_currentFrame + 1) % _framesInCurrentAnimation;
    _lastFrameTime = now;
}

void SpriteData::setAnimation(size_t animation) {
    if (animation >= _animationsInfo.size())
        throw std::invalid_argument("Animation cannot be greater than number of animation");

    _currentAnimation = animation;
    _currentFrame = 0UL;
    _lastFrameTime = ClockType::now();
    fetchAnimationsInfo();
}

void SpriteData::resetAnimation() {
    _currentFrame = 0UL;
    _lastFrameTime = ClockType::now();
}

}
