#pragma once

#include "Utils.h"

#include <SDL2/SDL.h>

bool AABBcast(const SDL_Rect& source, const SDL_Rect& target, const f2v& velocity, raycastHit& outhit, float deltaTime) {
    if (velocity.x == 0 && velocity.y == 0) return false;

    bool collisionX = false;
    bool collisionY = false;

    if (velocity.x != 0) {
        SDL_Rect futureX = source;
        futureX.x += static_cast<int>(velocity.x * deltaTime);

        if (SDL_HasIntersection(&futureX, &target)) {
            collisionX = true;
            outhit.normal.x = (velocity.x > 0) ? -1.0f : 1.0f;
        }
    }

    if (velocity.y != 0) {
        SDL_Rect futureY = source;
        futureY.y += static_cast<int>(velocity.y * deltaTime);

        if (SDL_HasIntersection(&futureY, &target)) {
            collisionY = true;
            outhit.normal.y = (velocity.y > 0) ? -1.0f : 1.0f;
        }
    }

    return collisionX || collisionY;
}
