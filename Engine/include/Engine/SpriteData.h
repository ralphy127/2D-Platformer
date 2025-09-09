#pragma once

#include <vector>
#include <utility>
#include <chrono>
#include "Engine/Clock.h"

namespace engine {

using AnimationId = uint8_t;
using FrameId = uint8_t;

/// @brief Holds animation data and manages frame progression for sprite animations.
class SpriteData {
public:
    /// @brief Vector containing pairs of (frame count, duration per frame) per animation.
    using AnimationsInfo = std::vector<std::pair<uint32_t, Clock::Type::duration>>;
    
    SpriteData() = default;
    SpriteData(AnimationsInfo animationsInfo, AnimationId initialAnimation);

    /// @brief Advances the current frame if enough time has passed according to animation timing.
    /// Should be called regularly (e.g. once per game update).
    void updateFrame();

    FrameId getFrame() const { return _currentFrame; }
    void setFrame(FrameId);

    AnimationId getAnimation() const { return _currentAnimation; }
    void setAnimation(AnimationId animation);

    uint32_t getAnimationCount() const { return _animationsInfo.size(); }

    uint32_t getFramesInCurrentAnimation() const { return _framesInCurrentAnimation; }

    Clock::Type::duration getCurrentAnimationDuration() const { return _targetFrameDuration; }

    /// @brief Resets the current animation to its first frame and resets timing.
    void resetAnimation();

private:
    /// @brief Updates information about frames count and target frame duration.
    void fetchAnimationsInfo();

    AnimationsInfo _animationsInfo;               ///< Pairs of (frames count, frame duration).
    AnimationId _currentAnimation;                ///< Current animation index.
    uint32_t _currentFrame{};                     ///< Current frame index within the animation.
    uint32_t _framesInCurrentAnimation{};         ///< Number of frames in the current animation.
    Clock::Type::duration _targetFrameDuration{}; ///< Duration to display each frame.
    Clock::Type::time_point _lastFrameTime;       ///< Time point of last frame update.
};

}
