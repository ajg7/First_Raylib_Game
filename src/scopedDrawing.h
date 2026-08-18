#pragma once

#include "raylib.h"

class ScopedDrawing {
public:
    ScopedDrawing() {
        BeginDrawing();
    }

    ~ScopedDrawing() {
        EndDrawing();
    }

    ScopedDrawing(const ScopedDrawing&) = delete;
    ScopedDrawing& operator=(const ScopedDrawing&) = delete;
    ScopedDrawing(ScopedDrawing&&) = delete;
    ScopedDrawing& operator=(ScopedDrawing&&) = delete;
};