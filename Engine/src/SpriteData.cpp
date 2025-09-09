#include "Engine/SpriteData.h"

#include <stdexcept>
#include <chrono>
#include <iostream>

namespace engine {

SpriteData::SpriteData(AnimationsInfo animationsInfo, AnimationId initialAnimation) 
    : _animationsInfo(std::move(animationsInfo)),
      _currentAnimation(initialAnimation),
      _lastFrameTime(Clock::getTime()) {
        
    if (_currentAnimation >= _animationsInfo.size())
        throw std::invalid_argument("Initial animation out of range");

    fetchAnimationsInfo();
}

void SpriteData::fetchAnimationsInfo() {
    _framesInCurrentAnimation = _animationsInfo.at(_currentAnimation).first;
    _targetFrameDuration = _animationsInfo.at(_currentAnimation).second;
}

void SpriteData::setFrame(FrameId id) {
    if (id >= _framesInCurrentAnimation)
        throw std::invalid_argument("Frame id cannot be greater than frames in current animation");

    _currentFrame = id;
    _lastFrameTime = Clock::getTime();
}

void SpriteData::updateFrame() {
    auto now = Clock::getTime();

    if (now - _lastFrameTime < _targetFrameDuration)
        return;

    _currentFrame = (_currentFrame + 1) % _framesInCurrentAnimation;
    _lastFrameTime = now;
}

void SpriteData::setAnimation(AnimationId animation) {
    if (animation >= _animationsInfo.size())
        throw std::invalid_argument("Animation cannot be greater than number of animation");

    _currentAnimation = animation;
    _currentFrame = 0UL;
    _lastFrameTime = Clock::getTime();
    fetchAnimationsInfo();
}

void SpriteData::resetAnimation() {
    _currentFrame = 0UL;
    _lastFrameTime = Clock::getTime();
}

}
