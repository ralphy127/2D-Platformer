#pragma once

#include <cstdint>

namespace engine {

/// @brief Interface for objects that can be updated each frame.
class IUpdatable {
public:
    virtual ~IUpdatable() = default;

    /// @brief Updates the object state based on elapsed time.
    virtual void update(float deltaTime) = 0;
};

}
