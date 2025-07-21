#pragma once

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace utils {

/// @brief Simple 2D integer vector with basic arithmetic operations and length calculation.
struct i2v {
    int x = 0, y = 0;

    i2v() = default;
    i2v(int x, int y) : x(x), y(y) {}

    float length() const { return std::sqrt(static_cast<float>(x * x + y * y)); }

    i2v operator+(const i2v& other) const { return i2v(x + other.x, y + other.y); }
    i2v operator-(const i2v& other) const { return i2v(x - other.x, y - other.y); }
    i2v operator*(const i2v& other) const { return i2v(x * other.x, y * other.y); }
    i2v operator/(const i2v& other) const {
        if (other.x == 0 || other.y == 0) throw std::runtime_error("Division by zero");
        return i2v(x / other.x, y / other.y); }

    i2v operator*(float scalar) const { return i2v(static_cast<int>(x * scalar), static_cast<int>(y * scalar)); }
    i2v operator/(float scalar) const {
        if (scalar == 0.f) throw std::runtime_error("Division by zero");
        return i2v(static_cast<int>(x / scalar), static_cast<int>(y / scalar)); }

    i2v& operator+=(const i2v& other) { x += other.x; y += other.y; return *this; }
    i2v& operator-=(const i2v& other) { x -= other.x; y -= other.y; return *this; }
    i2v& operator*=(int scalar) { x *= scalar; y *= scalar; return *this; }
    i2v& operator/=(int scalar) {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        x /= scalar; y /= scalar;
        return *this; }
    
    bool operator>(const i2v& other) const { return x > other.x && y > other.y; }
    bool operator<(const i2v& other) const { return x < other.x && y < other.y; }
    bool operator>(i2v&& other) const { return x > other.x && y > other.y; }
    bool operator<(i2v&& other) const { return x < other.x && y < other.y; }

    bool operator==(i2v& other) const { return x == other.x && y == other.y; }
    bool operator==(i2v&& other) const { return x == other.x && y == other.y; }
    bool operator!=(i2v& other) const { return !(*this == other); }
    bool operator!=(i2v&& other) const { return !(*this == other); }

    friend std::ostream& operator<<(std::ostream& os, const i2v& v) { return os << "i2v(" << v.x << ", " << v.y << ")"; }
};

}
