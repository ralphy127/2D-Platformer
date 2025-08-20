#pragma once

#include <vector>
#include <utility>
#include <chrono>
#include "Engine/Clock.h"

namespace engine {

/// @brief Holds animation data and manages frame progression for sprite animations.
class SpriteData {
public:
    /// @brief Vector containing pairs of (frame count, duration per frame) per animation.
    using AnimationsInfo = std::vector<std::pair<size_t, Clock::Type::duration>>;
    
    SpriteData() = default;
    SpriteData(AnimationsInfo animationsInfo, size_t initialAnimation);

    /// @brief Advances the current frame if enough time has passed according to animation timing.
    /// Should be called regularly (e.g. once per game update).
    void updateFrame();

    size_t getFrame() const { return _currentFrame; }
    void setFrame(size_t frame);

    size_t getAnimation() const { return _currentAnimation; }
    void setAnimation(size_t animation);

    size_t getAnimationCount() const { return _animationsInfo.size(); }

    size_t getFramesInCurrentAnimation() const { return _framesInCurrentAnimation; }

    Clock::Type::duration getCurrentAnimationDuration() const { return _targetFrameDuration; }

    /// @brief Resets the current animation to its first frame and resets timing.
    void resetAnimation();

private:
    /// @brief Updates information about frames count and target frame duration.
    void fetchAnimationsInfo();

    AnimationsInfo _animationsInfo;               ///< Pairs of (frames count, frame duration).
    size_t _currentAnimation;                     ///< Current animation index.
    size_t _currentFrame{};                       ///< Current frame index within the animation.
    size_t _framesInCurrentAnimation{};           ///< Number of frames in the current animation.
    Clock::Type::duration _targetFrameDuration{}; ///< Duration to display each frame.
    Clock::Type::time_point _lastFrameTime;       ///< Time point of last frame update.
};

}
