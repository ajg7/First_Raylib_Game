#pragma once

#include "config.h"
#include "raylib.h"
#include "animation.h"

struct Layer 
{
    Texture2D texture;
    float parallax;
    
    static Layer load(const Config::Assets::Backdrop::LayerDef& def) {
        return Layer{ LoadTexture(def.path), def.parallax };
    }
    
    void draw(const float cameraOffsetX = 0.0f) const {
        const float x = -cameraOffsetX * parallax; // Horizontal offset scaled by the layer's parallax factor
        const Rectangle src{ 0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
        const Rectangle dst{ x, 0.0f, Config::Virtual::WidthF, Config::Virtual::HeightF };
        
        DrawTexturePro(texture, src, dst, {}, 0.0f, WHITE);
    }

    static Rectangle frameRect(const Animation& anim, const int frameWidth, const int frameHeight, const bool facingLeft) {
    const float x = facingLeft ? static_cast<float>(frameWidth) * (anim.frameIndex + 1) : static_cast<float>(frameWidth) * anim.frameIndex;
    const float w = facingLeft ? -static_cast<float>(frameWidth) : static_cast<float>(frameWidth);
    return Rectangle{ x, 0.0f, w, static_cast<float>(frameHeight) };
}
};