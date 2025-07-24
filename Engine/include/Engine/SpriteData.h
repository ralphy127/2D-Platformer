#pragma once

#include <vector>
#include <utility>
#include <chrono>

namespace engine {

/// @brief Holds animation data and manages frame progression for sprite animations.
class SpriteData {
public:
    using ClockType = std::chrono::steady_clock;

    /// @brief Alias for a vector containing pairs of (frame count, duration per frame) per animation.
    using AnimationsInfo = std::vector<std::pair<size_t, ClockType::duration>>;
    
    /// @brief Default constructor creates empty SpriteData with no animations.
    SpriteData() = default;

    /// @brief Constructs SpriteData with animations info and initial animation index.
    /// @param animationsInfo Vector of pairs: (frames in animation, frame duration).
    /// @param initialAnimation Index of the animation to start with.
    SpriteData(AnimationsInfo animationsInfo, size_t initialAnimation);

    /// @brief Returns the current frame index within the current animation.
    /// @return Current frame number (0-based).
    size_t getFrame() const { return _currentFrame; }

    /// @brief Sets the current frame index within the current animation.
    /// Clamps to valid range.
    /// @param frame Frame index to set.
    void setFrame(size_t frame);

    /// @brief Advances the current frame if enough time has passed according to animation timing.
    /// Should be called regularly (e.g. once per game update).
    void updateFrame();

    /// @brief Returns the current animation index.
    /// @return Current animation number (0-based).
    size_t getAnimation() const { return _currentAnimation; }

    /// @brief Changes the current animation to the specified one, resetting frame and timing.
    /// Clamps to valid range.
    /// @param animation Animation index to set.
    void setAnimation(size_t animation);

    /// @brief Returns how many animations are stored.
    /// @return Total number of animations.
    size_t getAnimationCount() const { return _animationsInfo.size(); }

    /// @brief Returns the number of frames in the current animation.
    /// @return Frame count of the current animation.
    size_t getFramesInCurrentAnimation() const { return _framesInCurrentAnimation; }

    /// @brief Returns the duration each frame should be displayed in the current animation.
    /// @return Duration of a single frame in the current animation.
    ClockType::duration getCurrentAnimationDuration() const { return _targetFrameDuration; }

    /// @brief Resets the current animation to its first frame and resets timing.
    void resetAnimation();

private:
    /// @brief Updates cached info like frames count and target frame duration based on the current animation.
    void fetchAnimationsInfo();

    /// @brief Stores pairs of (frames count, frame duration) for each animation.
    AnimationsInfo _animationsInfo;

    size_t _currentAnimation{};                 ///< Current animation index.
    size_t _currentFrame{};                     ///< Current frame index within the animation.
    size_t _framesInCurrentAnimation{};         ///< Cached number of frames in the current animation.
    ClockType::duration _targetFrameDuration{}; ///< Duration to display each frame in the current animation.
    ClockType::time_point _lastFrameTime{};     ///< Time point of last frame update.
};

}
