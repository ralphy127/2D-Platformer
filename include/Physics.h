#pragma once

#include "Utils.h"

#include <SDL2/SDL.h>

bool raycast(const ray& ray, const SDL_Rect& target, raycastHit& outHit, float maxDistance) {
    f2v targetPos(target.x, target.y);

    f2v tNear = (targetPos - ray.origin) / ray.direction;
    f2v tFar = (targetPos + f2v(target.w, target.h) - ray.origin) / ray.direction;

    if(std::isnan(tNear.x) || std::isnan(tNear.y) || std::isnan(tFar.x) || std::isnan(tFar.y)) {
        return false;
    }

    if(tNear.x > tFar.x) {
        std::swap(tNear.x, tFar.x);
        std::swap(tNear.y, tFar.y);
    }

    if(tNear.x > tFar.y || tNear.y > tFar.x) {
        return false;
    }

    float tHitNear = fmax(tNear.x, tNear.y);
    float tHitFar = fmin(tFar.x, tFar.y);

    if(tHitFar < 0.f) {
        return false;
    }
    
    outHit.point = ray.origin + ray.direction * tHitNear;
    outHit.distance = tHitNear;

    if(tNear.x > tNear.y) {
        if(ray.direction.x < 0.f) {
            outHit.normal = {1.f, 0.f};
        } 
        else {
            outHit.normal = {-1.f, 0.f};
        }
    }
    else if(tNear.x < tNear.y) {
        if(ray.direction.y < 0.f) {
            outHit.normal = {0.f, 1.f};
        }
        else {
            outHit.normal = {0.f, -1.f};
        }
    }

    return (tHitNear < maxDistance);
}

/*bool AABBcast(SDL_Rect& source, SDL_Rect& target, f2v& direction, raycastHit& outhit, float maxDistance) {
    ray ray({source.x + source.w/2.f, source.y + source.h/2.f}, direction);

    SDL_Rect expandedTarget = target;
    expandedTarget.x -= source.w/2.f;
    expandedTarget.y -= source.h/2.f;
    expandedTarget.w += source.w;
    expandedTarget.h += source.h;

    return raycast(ray, expandedTarget, outhit, maxDistance);
}*/

/*bool AABBcast(const SDL_Rect& source, const SDL_Rect& target, const f2v& direction, raycastHit& outhit, float maxDistance) {
    if (direction.x == 0 && direction.y == 0) return false; // brak ruchu
    
    // Normalizacja kierunku
    f2v normalized_dir = direction;
    float length = sqrt(normalized_dir.x * normalized_dir.x + normalized_dir.y * normalized_dir.y);
    if (length > 0) {
        normalized_dir.x /= length;
        normalized_dir.y /= length;
    }
    
    ray ray({source.x + source.w/2.f, source.y + source.h/2.f}, normalized_dir);
    
    // Oblicz rozszerzony cel uwzględniający rozmiar źródła
    SDL_Rect expandedTarget = {
        target.x - source.w/2,
        target.y - source.h/2,
        target.w + source.w,
        target.h + source.h
    };
    
    return raycast(ray, expandedTarget, outhit, maxDistance);
}*/

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