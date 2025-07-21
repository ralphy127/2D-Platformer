#pragma once

namespace engine {

class IUpdatable {
public:
    virtual void update(float deltaTime) = 0;
    virtual ~IUpdatable() = default;
};

}