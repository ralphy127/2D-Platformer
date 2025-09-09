#pragma once

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace utils {

struct f2v;

struct i2v {
    int32_t x = 0, y = 0;

    i2v() = default;
    i2v(int32_t x, int32_t y) : x(x), y(y) {}

    f2v tof2v() const;

    float length() const { return std::sqrt(static_cast<float>(x * x + y * y)); }

    i2v operator+(const i2v& other) const { return i2v(x + other.x, y + other.y); }
    i2v operator-(const i2v& other) const { return i2v(x - other.x, y - other.y); }
    i2v operator*(const i2v& other) const { return i2v(x * other.x, y * other.y); }
    i2v operator/(const i2v& other) const {
        if (other.x == 0 || other.y == 0) throw std::runtime_error("Division by zero");
        return i2v(x / other.x, y / other.y); }

    i2v operator*(float scalar) const {
        return i2v(static_cast<int32_t>(x * scalar), static_cast<int32_t>(y * scalar)); }
    i2v operator/(float scalar) const {
        if (scalar == 0.f) throw std::runtime_error("Division by zero");
        return i2v(static_cast<int32_t>(x / scalar), static_cast<int32_t>(y / scalar)); }

    i2v& operator+=(const i2v& other) { x += other.x; y += other.y; return *this; }
    i2v& operator-=(const i2v& other) { x -= other.x; y -= other.y; return *this; }
    i2v& operator*=(int32_t scalar) { x *= scalar; y *= scalar; return *this; }
    i2v& operator/=(int32_t scalar) {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        x /= scalar; y /= scalar;
        return *this; }
    
    bool operator>(const i2v& other) const { return length() > other.length(); }
    bool operator<(const i2v& other) const { return length() < other.length(); }
    bool operator>(i2v&& other) const { return length() > other.length(); }
    bool operator<(i2v&& other) const { return length() < other.length(); }

    bool operator==(const i2v& other) const { return x == other.x && y == other.y; }
    bool operator==(i2v&& other) const { return x == other.x && y == other.y; }
    bool operator!=(const i2v& other) const { return x != other.x || y != other.y; }
    bool operator!=(i2v&& other) const { return x != other.x || y != other.y; }

    friend std::ostream& operator<<(std::ostream& os, const i2v& v) {
        return os << "[" << v.x << ", " << v.y << "]"; }
};

}
