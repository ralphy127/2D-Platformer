#pragma once

#include <vector>
#include <utility>
#include <chrono>

namespace engine {

// frame, animation indexed from 0
class SpriteData {
public:
    using ClockType = std::chrono::steady_clock;
    using AnimationsInfo = std::vector<std::pair<size_t, ClockType::duration>>;
    
    SpriteData() = default;
    SpriteData(AnimationsInfo, size_t initialAnimation);

    size_t getFrame() const { return _currentFrame; }
    void setFrame(size_t frame);

    void updateFrame();

    size_t getAnimation() const { return _currentAnimation; }
    void setAnimation(size_t animation);

    size_t getAnimationCount() const { return _animationsInfo.size(); }
    size_t getFramesInCurrentAnimation() const { return _framesInCurrentAnimation; }
    ClockType::duration getCurrentAnimationDuration() const { return _targetFrameDuration; }

    void resetAnimation();
private:
    void fetchAnimationsInfo();
    // [animation] = pair<(frames in animations), (target frame time of that animation) 
    AnimationsInfo _animationsInfo;
    size_t _currentAnimation{};
    size_t _currentFrame{};
    size_t _framesInCurrentAnimation{};
    ClockType::duration _targetFrameDuration{};
    ClockType::time_point _lastFrameTime{};
};

}
