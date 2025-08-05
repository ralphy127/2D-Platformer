#include "Utils/f2v.h"

#include "Utils/i2v.h"

namespace utils {

i2v f2v::toi2v() const { return i2v(static_cast<int>(x), static_cast<int>(y)); }

void f2v::normalizeInPlace() {
    const auto len = length();

    if (std::abs(len) > std::numeric_limits<float>::epsilon()) {
        x /= len;
        y /= len;
    }
}

f2v f2v::operator/(const f2v& other) const {
    if (std::abs(other.x) <= std::numeric_limits<float>::epsilon() ||
        std::abs(other.y) <= std::numeric_limits<float>::epsilon() ) {

        throw std::runtime_error("Division by zero");
    }

    return f2v(x / other.x, y / other.y); 
}

f2v f2v::operator/(float scalar) const {
    if (std::abs(scalar) <= std::numeric_limits<float>::epsilon())
        throw std::runtime_error("Division by zero");

    return f2v(x / scalar, y / scalar);
}

f2v& f2v::operator/=(float scalar) {
    if (std::abs(scalar) <= std::numeric_limits<float>::epsilon())
        throw std::runtime_error("Division by zero");

    x /= scalar;
    y /= scalar;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const f2v& v) {
    return os << "[" << v.x << ", " << v.y << "]"; }
    
}