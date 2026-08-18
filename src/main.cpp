#include "raylib.h"
#include "raymath.h"
#include "frame_rates_monitor.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

// The internal coordinate space everything is authored and simulated in.
// Every entity position and collision box downstream is measured in these
// pixels, not window pixels.
#define VIRTUAL_WIDTH  480
#define VIRTUAL_HEIGHT 270

#define GROUND_LINE_Y 131

typedef struct Layer 
{
    Texture2D texture;
    float parallax;
} Layer;

int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gloom & Utter Darkness");
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    Layer layers[5] = {
        { LoadTexture("assets/backdrop/bg.png"),           0.2f },
        { LoadTexture("assets/backdrop/rail_wall.png"),     0.4f },
        { LoadTexture("assets/backdrop/columns_floor.png"), 0.7f },
        { LoadTexture("assets/backdrop/wires.png"),         0.9f },
        { LoadTexture("assets/backdrop/train.png"),         1.0f },
    };
    float cameraOffsetX = 0.0f;
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    // Fit the virtual resolution into the window, preserving aspect ratio,
    // and centre it (letterbox/pillarbox if the ratios don't match exactly).
    float scale = fminf((float)WINDOW_WIDTH / VIRTUAL_WIDTH, (float)WINDOW_HEIGHT / VIRTUAL_HEIGHT);
    Rectangle srcRect = { 0.0f, 0.0f, (float)VIRTUAL_WIDTH, -(float)VIRTUAL_HEIGHT };
    Rectangle dstRect = {
        (WINDOW_WIDTH - VIRTUAL_WIDTH * scale) * 0.5f,
        (WINDOW_HEIGHT - VIRTUAL_HEIGHT * scale) * 0.5f,
        VIRTUAL_WIDTH * scale,
        VIRTUAL_HEIGHT * scale
    };

    while (!WindowShouldClose())
    {
        BeginFrameRateMonitor();
        BeginTextureMode(target);
            ClearBackground(BLACK);
            for (int i = 0; i < 5; i++) {
                float x = -cameraOffsetX * layers[i].parallax;
                DrawTexturePro(
                    layers[i].texture,
                    (Rectangle){ 0, 0, (float)layers[i].texture.width, (float)layers[i].texture.height },
                    (Rectangle){ x, 0, (float)VIRTUAL_WIDTH, (float)VIRTUAL_HEIGHT },
                    (Vector2){ 0, 0 }, 0.0f, WHITE);
            }
            DrawLine(0, GROUND_LINE_Y, VIRTUAL_WIDTH, GROUND_LINE_Y, RED);
            DrawFrameRateText();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(target.texture, srcRect, dstRect, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }

    for (int i = 0; i < 5; i++) {
        UnloadTexture(layers[i].texture);
    }

    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
