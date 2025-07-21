#pragma once

namespace engine {

class ITileClassifier {
public:
    virtual ~ITileClassifier() = default;

    virtual std::pair<int, int> classify(int tileId) const = 0;
};

}
