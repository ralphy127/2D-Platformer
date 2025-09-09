#pragma once

#include <cmath>
#include <ostream>

namespace utils {

struct i2v;

struct f2v {
    float x{};
    float y{};

    f2v() = default;
    f2v(float x, float y) : x(x), y(y) {}

    i2v toi2v() const;

    float length() const { return std::sqrt(x * x + y * y); }
    void normalizeInPlace();

    f2v operator+(const f2v& other) const { return {x + other.x, y + other.y}; }
    f2v operator-(const f2v& other) const { return {x - other.x, y - other.y}; }
    f2v operator*(const f2v& other) const { return {x * other.x, y * other.y}; }
    f2v operator/(const f2v& other) const;

    f2v operator*(float scalar) const { return {x * scalar, y * scalar}; }
    f2v operator/(float scalar) const;

    f2v& operator+=(const f2v& other) { x += other.x; y += other.y; return *this; }
    f2v& operator-=(const f2v& other) { x -= other.x; y -= other.y; return *this; }
    f2v& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    f2v& operator/=(float scalar);

    bool operator==(const f2v& other) const { return x == other.x && y == other.y; }
    bool operator!=(const f2v& other) const { return !(*this == other); }

    bool operator<(const f2v& other) const { return length() < other.length(); }
    bool operator>(const f2v& other) const { return other < *this; }
    bool operator<=(const f2v& other) const { return !(other < *this); }
    bool operator>=(const f2v& other) const { return !(*this < other); }

    friend std::ostream& operator<<(std::ostream& os, const f2v& v);
};

inline f2v operator*(float scalar, const f2v& v) {
    return v * scalar;
}

}
