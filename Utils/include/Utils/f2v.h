#pragma once

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace utils {

/// @brief Simple 2D float vector with basic arithmetic operations and length calculation.
struct f2v {
    float x = 0.f, y = 0.f;

    f2v() = default;
    f2v(float x, float y) : x(x), y(y) {}

    float length() const { return std::sqrt(x * x + y * y); }

    void normalizeInPlace() {
        float len = length();
        if (len > 1e-6f) { x /= len; y /= len; }}

    f2v operator+(const f2v& other) const { return f2v(x + other.x, y + other.y); }
    f2v operator-(const f2v& other) const { return f2v(x - other.x, y - other.y); }
    f2v operator*(const f2v& other) const { return f2v(x * other.x, y * other.y); }
    f2v operator/(const f2v& other) const {
        if (other.x == 0.f || other.y == 0.f) throw std::runtime_error("Division by zero");
        return f2v(x / other.x, y / other.y); }

    f2v operator*(float scalar) const { return f2v(x * scalar, y * scalar); }
    f2v operator/(float scalar) const {
        if (scalar == 0.f) throw std::runtime_error("Division by zero");
        return f2v(x / scalar, y / scalar); }

    f2v& operator+=(const f2v& other) { x += other.x; y += other.y; return *this; }
    f2v& operator-=(const f2v& other) { x -= other.x; y -= other.y; return *this; }
    f2v& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    f2v& operator/=(float scalar) {
        if (scalar == 0.f) throw std::runtime_error("Division by zero");
        x /= scalar; y /= scalar;
        return *this; }
    
    bool operator>(const f2v& other) const { return length() > other.length(); }
    bool operator<(const f2v& other) const { return length() < other.length(); }
    bool operator>(f2v&& other) const { return length() > other.length(); }
    bool operator<(f2v&& other) const { return length() < other.length(); }  

    bool operator<=(const f2v& other) const { return length() <= other.length(); }
    bool operator>=(const f2v& other) const { return length() >= other.length(); }
    bool operator<=(f2v&& other) const { return length() <= other.length(); }
    bool operator>=(f2v&& other) const { return length() >= other.length(); }
    
    bool operator==(const f2v& other) const { return x == other.x && y == other.y; }
    bool operator==(f2v&& other) const { return x == other.x && y == other.y; }
    bool operator!=(const f2v& other) const { return x != other.x || y != other.y; }
    bool operator!=(f2v&& other) const { return x != other.x || y != other.y; }

    friend std::ostream& operator<<(std::ostream& os, const f2v& v) {
        return os << "[" << v.x << ", " << v.y << "]"; }
};

}
