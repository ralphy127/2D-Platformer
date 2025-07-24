#pragma once

namespace engine {

/// @brief Interface for objects that can be updated each frame.
class IUpdatable {
public:
    virtual ~IUpdatable() = default;

    /// @brief Updates the object state based on elapsed time.
    /// @param deltaTime Time elapsed since last update (in seconds).
    virtual void update(float deltaTime) = 0;
};

}
