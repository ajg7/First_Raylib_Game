#include "raylib.h"

#include "config.h"
#include "frameRateMonitor.h"
#include "scopedDrawing.h"
#include "scopedTextureMode.h"
#include "scopedWindow.h"
#include "scopedRenderTexture.h"
#include "scopedLayers.h"

#include <cstdlib>

int main(void)
{
    // Window
    const ScopedWindow window{Config::Window::Width, Config::Window::Height, "Dazzle's Darkness"};
    SetTargetFPS(Config::Window::TargetFPS);

    // Virtual Canvas
    const ScopedRenderTexture target{Config::Virtual::Width, Config::Virtual::Height};
    SetTextureFilter(target.getTexture(), TEXTURE_FILTER_POINT);

    // Background Layers
    const ScopedLayers backgroundLayers{Config::Assets::Backdrop::All};

    FrameRateMonitor frameRateMonitor{};

    // Game Loop
    while (not WindowShouldClose())
    {
        frameRateMonitor.update(GetFrameTime());
        
        // -------------------------------------------------------------------------
        // Virtual Canvas Pass
        // Render all game content into the low-resolution virtual render texture.
        // -------------------------------------------------------------------------
        {
            const ScopedTextureMode textureMode{target};
            ClearBackground(BLACK);
            
            // Draw Background Layers
            for (const auto& layer : backgroundLayers) {
                layer.draw();
            }
            
            // Draw Debug Overlays
            DrawLine(0, Config::World::GroundLineY, Config::Virtual::Width, Config::World::GroundLineY, RED);
            frameRateMonitor.drawFrameRateText();
        }

        // -------------------------------------------------------------------------
        // Main Window Upscale Pass
        // Scales the virtual canvas up to the physical window size with letterboxing applied
        // -------------------------------------------------------------------------
        {
            const ScopedDrawing drawing;
            ClearBackground(BLACK);
            
            DrawTexturePro(
                target.getTexture(), 
                Config::Viewport::SrcRect, 
                Config::Viewport::DstRect, 
                {}, 
                0.0f, 
                WHITE
            );
        }
    }

    return EXIT_SUCCESS;
}