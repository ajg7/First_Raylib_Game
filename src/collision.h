#pragma once
#include <optional>
#include "raylib.h"
#include <algorithm>

//AABB = Axis-Aligned Bounding Box
std::optional<float> sweptAABB(Vector2 segStart, Vector2 segEnd, Rectangle expandedBox) {
  float farEdgeX = ((expandedBox.x + expandedBox.width) - segStart.x) / (segEnd.x - segStart.x);
  float nearEdgeX = (expandedBox.x - segStart.x) / (segEnd.x - segStart.x);
  float farEdgeY = ((expandedBox.y + expandedBox.height) - segStart.y) / (segEnd.y - segStart.y);
  float nearEdgeY = (expandedBox.y - segStart.y) / (segEnd.y - segStart.y);

  float xEnter = std::min(farEdgeX, nearEdgeX);
  float xExit = std::max(farEdgeX, nearEdgeX);

  float yEnter = std::min(farEdgeY, nearEdgeY);
  float yExit = std::max(farEdgeY, nearEdgeY);

  float entry = std::max(xEnter, yEnter);
  float exit = std::min(xExit, yExit);

  if (entry > exit || exit < 0 || entry > 1) return std::nullopt;
  return entry;
}