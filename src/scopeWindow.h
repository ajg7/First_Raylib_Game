#pragma once

#include "raylib.h"

struct ScopedWindow {
    ScopedWindow(const int width, const int height, const char* title) {
        InitWindow(width, height, title);
    }

    ~ScopedWindow() {
        CloseWindow();
    }

    ScopedWindow(const ScopedWindow&) = delete;
    ScopedWindow& operator=(const ScopedWindow&) = delete;
    ScopedWindow(ScopedWindow&&) = delete;
    ScopedWindow& operator=(ScopedWindow&&) = delete;
};