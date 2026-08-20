#pragma once

#include "raylib.h"

struct Bullet {
    Vector2 position;
    Vector2 velocity;

    void update(const double simDt) {
        position.x += velocity.x * static_cast<float>(simDt);
        position.y += velocity.y * static_cast<float>(simDt);
    }
};