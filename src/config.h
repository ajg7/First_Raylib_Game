#pragma once

#include <array>
#include "raylib.h"

// Configuration constants for the physical application window
namespace Config::Window {
    constexpr int Width{1280};
    constexpr int Height{720};
    constexpr int TargetFPS{60};

    constexpr float WidthF{static_cast<float>(Width)};
    constexpr float HeightF{static_cast<float>(Height)};
}

// Configuration constants for the virtual canvas resolution
namespace Config::Virtual {
    // The internal coordinate space everything is authored and simulated in.
    // Every entity position and collision box downstream is measured in these
    // pixels, not window pixels.
    constexpr int Width{480};
    constexpr int Height{270};

    constexpr float WidthF{static_cast<float>(Width)};
    constexpr float HeightF{static_cast<float>(Height)};
}

// Global world layout measurements within the virtual coordinate space
namespace Config::World {
    constexpr int GroundLineY{131};
    constexpr int ArenaMinX{0};
    constexpr int ArenaMaxX{Config::Virtual::Width};
}

// Viewport scaling and letterboxing math to fit the virtual canvas onto the physical window
namespace Config::Viewport {
    // Uniform scale factor preserving aspect ratio between virtual space and window dimensions
    constexpr float Scale = std::min(Window::WidthF / Virtual::WidthF, 
                                     Window::HeightF / Virtual::HeightF);

    // Source rectangle mapping the entire virtual canvas (flipped Y for OpenGL texture coordinates)
    constexpr Rectangle SrcRect{0.0f, 0.0f, Virtual::WidthF, -Virtual::HeightF};

    // Destination rectangle centered on the physical window with letterboxing/pillarboxing applied
    constexpr Rectangle DstRect{
        (Window::WidthF - Virtual::WidthF * Scale) * 0.5f,
        (Window::HeightF - Virtual::HeightF * Scale) * 0.5f,
        Virtual::WidthF * Scale,
        Virtual::HeightF * Scale
    };
}

// Asset definitions for background parallax layers
namespace Config::Assets::Backdrop {
    // Defines metadata for a single backdrop layer image and its parallax scroll factor
    struct LayerDef {
        const char* path;
        float parallax;
    };

    constexpr LayerDef Bg{ "assets/backdrop/bg.png", 0.2f };
    constexpr LayerDef RailWall{ "assets/backdrop/rail_wall.png", 0.4f };
    constexpr LayerDef ColumnsFloor{ "assets/backdrop/columns_floor.png", 0.7f };
    constexpr LayerDef Wires{ "assets/backdrop/wires.png", 0.9f };
    constexpr LayerDef Train{ "assets/backdrop/train.png", 1.0f };

    // Consolidated array of all backdrop layer definitions in render order (back-to-front)
    constexpr std::array All{ Bg, RailWall, ColumnsFloor, Wires, Train };
}

namespace Config::Assets::Scientist {
    const char* idlePath{"assets/scientist/idle.png"};
    const char* walkPath{"assets/scientist/walk.png"};
    const char* specialPath{"assets/scientist/special.png"};
    constexpr int frameWidth{128};
    constexpr int frameHeight{128};
    constexpr int idleFrameCount{6};
    constexpr int walkFrameCount{12};
    constexpr double idleFrameDuration{0.15};
    constexpr double walkFrameDuration{0.08};
    constexpr float moveSpeed{60.0f};
    constexpr int specialFrameCount{14};
    constexpr double specialFrameDuration{0.07};
    constexpr int specialFlashFrame{2};
    constexpr double fireCooldown{0.25};
}

namespace Config::Gameplay {
    constexpr float bulletSpeed{300.0f};
}

namespace Config::Assets::Zombie {
    const char* walkPath{"assets/zombie/walk.png"};
    const char* deadPath{"assets/zombie/dead.png"};
    constexpr int frameWidth{96};
    constexpr int frameHeight{96};
    constexpr int walkFrameCount{10};
    constexpr double walkFrameDuration{0.1};
    constexpr int deadFrameCount{5};
    constexpr double deadFrameDuration{0.12};
    constexpr float moveSpeed{25.0f};
    constexpr int startHp{3};
}
