#ifndef FRAME_RATES_MONITOR_H
#define FRAME_RATES_MONITOR_H

#include <raylib.h>

#define SIM_HZ 60.0
#define SIM_DT (1.0 / SIM_HZ)

static double accumulator = 0.0;
static unsigned long long simTicks = 0;
static unsigned long long renderFrames = 0;

void BeginFrameRateMonitor() {
  accumulator += GetFrameTime();
  while (accumulator >= SIM_DT)
  {
      // Simulation update will go here.
      simTicks++;
      accumulator -= SIM_DT;
  }
  renderFrames++;
}
void DrawFrameRateText() {
  DrawText(TextFormat("sim ticks:   %llu", simTicks), 4, 4, 10, GREEN);
  DrawText(TextFormat("render frames: %llu", renderFrames), 4, 16, 10, GREEN);
}

#endif // FRAME_RATES_MONITOR_H