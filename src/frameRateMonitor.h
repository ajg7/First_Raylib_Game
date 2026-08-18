#pragma once

#include <cstdint>
#include <format>
#include <raylib.h>

class FrameRateMonitor {
public:
    // Screen coordinate pair for UI positioning
    struct Position {
        int x;
        int y;
    };

    // Typography options for frame rate rendering
    struct FontOptions {
        int fontSize;
        Color color;
        int lineSpacing;
    };

    // Fixed-timestep target (60 Hz / ~16.6ms per tick)
    static constexpr double SIM_HZ{60.0};
    static constexpr double SIM_DT{1.0 / SIM_HZ};

    // Accumulates delta time and steps the simulation counter in fixed increments
    void update(const double deltaTime) noexcept {
        m_accumulator += deltaTime;
        while (m_accumulator >= SIM_DT) {
            ++m_simTicks;
            m_accumulator -= SIM_DT;
        }
        ++m_renderFrames;
    }

    // Renders the current tick and frame stats to the screen
    void drawFrameRateText(
        Position position = {.x = 4, .y = 4},
        FontOptions font = {.fontSize = 10, .color = GREEN, .lineSpacing = 2}
    ) const {
        const auto simTicksText = std::format("sim ticks: {}", m_simTicks);
        const auto renderFramesText = std::format("render frames: {}", m_renderFrames);

        DrawText(simTicksText.c_str(), position.x, position.y, font.fontSize, font.color);
        DrawText(renderFramesText.c_str(), position.x, position.y + font.fontSize + font.lineSpacing, font.fontSize, font.color);
    }

private:
    double m_accumulator{0.0};       // Unprocessed time buffer awaiting the next sim tick
    std::uint64_t m_simTicks{0};     // Total fixed-step updates executed
    std::uint64_t m_renderFrames{0}; // Total render frames executed
};