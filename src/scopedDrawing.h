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

    // Delete copy and move semantics to enforce strict RAII scope-bound ownership
    ScopedDrawing(const ScopedDrawing&) = delete;
    ScopedDrawing& operator=(const ScopedDrawing&) = delete;
    ScopedDrawing(ScopedDrawing&&) = delete;
    ScopedDrawing& operator=(ScopedDrawing&&) = delete;
};