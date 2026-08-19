#pragma once

#include "raylib.h"

struct ScopedRenderTexture {
    RenderTexture2D texture{};

    ScopedRenderTexture(const int width, const int height)
        : texture(LoadRenderTexture(width, height)) {}

    ~ScopedRenderTexture() {
        UnloadRenderTexture(texture);
    }

    operator RenderTexture2D() const { return texture; }
    
    const Texture2D& getTexture() const { return texture.texture; }

    ScopedRenderTexture(const ScopedRenderTexture&) = delete;
    ScopedRenderTexture& operator=(const ScopedRenderTexture&) = delete;
    ScopedRenderTexture(ScopedRenderTexture&&) = delete;
    ScopedRenderTexture& operator=(ScopedRenderTexture&&) = delete;
};