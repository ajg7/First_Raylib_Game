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

    ScopedTextureMode(const ScopedTextureMode&) = delete;
    ScopedTextureMode& operator=(const ScopedTextureMode&) = delete;
    ScopedTextureMode(ScopedTextureMode&&) = delete;
    ScopedTextureMode& operator=(ScopedTextureMode&&) = delete;
};