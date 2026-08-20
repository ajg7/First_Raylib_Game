#include "raylib.h"

#include "config.h"
#include "frameRateMonitor.h"
#include "scopedDrawing.h"
#include "scopedTextureMode.h"
#include "scopedWindow.h"
#include "scopedRenderTexture.h"
#include "scopedLayers.h"
#include "animation.h"
#include "player.h"
#include "bullet.h"
#include "zombie.h"
#include <vector>

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

    Player player{Player::load(Vector2{Config::Virtual::WidthF * 0.5f - 64.0f, static_cast<float>(Config::World::GroundLineY) - 128.0f})};
    Zombie zombie{Zombie::load(Vector2{
        static_cast<float>(Config::World::ArenaMaxX - Config::Assets::Zombie::frameWidth),
        static_cast<float>(Config::World::GroundLineY) - static_cast<float>(Config::Assets::Zombie::frameHeight)
    })};
    std::vector<Bullet> bullets;

    FrameRateMonitor frameRateMonitor{};

    // Game Loop
    while (not WindowShouldClose())
    {
        const int simSteps = frameRateMonitor.update(GetFrameTime());

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

            // Update Player, Zombie, and Bullets — once per simulated tick, not per render frame
            for (int step = 0; step < simSteps; ++step) {
                if (auto spawned = player.update(frameRateMonitor.SIM_DT)) bullets.push_back(*spawned);

                zombie.update(frameRateMonitor.SIM_DT, player.position.x);

                for (auto& bullet : bullets) bullet.update(frameRateMonitor.SIM_DT);

                if (!zombie.dead) {
                    for (auto it = bullets.begin(); it != bullets.end(); ) {
                        if (zombie.containsPoint(it->position)) {
                            zombie.takeHit();
                            it = bullets.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }
            }

            // Draw Bullets, Zombie, and Player
            for (const auto& bullet : bullets) {
                DrawRectangle(static_cast<int>(bullet.position.x), static_cast<int>(bullet.position.y), 4, 2, YELLOW);
            }
            zombie.draw();
            player.draw();

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