#pragma once

struct Animation {
  int frameCount;
  double frameDuration;
  double elapsed{0.0};
  int frameIndex{0};

  void advance(const double simDt) {
    elapsed += simDt;
    while (elapsed >= frameDuration) {
      elapsed -= frameDuration;
      frameIndex = (frameIndex + 1) % frameCount;
    }
  }

  void reset() {
    elapsed = 0.0;
    frameIndex = 0;
  }
};