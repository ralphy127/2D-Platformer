#pragma once

#include <cmath>

struct i2v {
    int x, y;
    i2v(int x = 0, int y = 0) : x(x), y(y) {}
};

struct f2v {
    float x, y;
    f2v(float x = 0.f, float y = 0.f) : x(x), y(y) {}

    void normalizeInPlace() {
        float length = this->length();
        if(length != 0.f) {
            x /= length;
            y /= length;
        }
    }

    friend f2v operator+(const f2v& a, const f2v& b) {
        return f2v(a.x + b.x, a.y + b.y);
    }

    friend f2v operator-(const f2v& a, const f2v& b) {
        return f2v(a.x - b.x, a.y - b.y);
    }

    friend f2v operator*(const f2v& a, const f2v& b) {
        return f2v(a.x * b.x, a.y * b.y);
    }

    friend f2v operator/(const f2v& a, const f2v& b) {
        return f2v(a.x / b.x, a.y / b.y);
    }

    float length() {
        return std::sqrt(x * x + y * y);
    }
};

struct ray {
    f2v origin;
    f2v direction;

    ray(const f2v& origin, const f2v& direction) : origin(origin), direction(direction) {
        this->direction.normalizeInPlace();
    }

    static ray fromTo(f2v& from, f2v& to) {
        return ray(from, to - from);
    }
};

struct raycastHit {
    f2v point;
    f2v normal;
    float distance;
};