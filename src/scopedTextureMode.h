#pragma once

#include "raylib.h"

class ScopedTextureMode {
public:
    explicit ScopedTextureMode(const RenderTexture2D& target) {
        BeginTextureMode(target);
    }

    ~ScopedTextureMode() {
        EndTextureMode();
    }

    // Delete copy and move semantics to enforce strict RAII scope-bound ownership
    ScopedTextureMode(const ScopedTextureMode&) = delete;
    ScopedTextureMode& operator=(const ScopedTextureMode&) = delete;
    ScopedTextureMode(ScopedTextureMode&&) = delete;
    ScopedTextureMode& operator=(ScopedTextureMode&&) = delete;
};