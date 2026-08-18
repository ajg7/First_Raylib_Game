#pragma once

#include <array>
#include <tuple>
#include "layer.h"
#include "raylib.h"

template <std::size_t N>
struct ScopedLayers {
    std::array<Layer, N> layers;

    template <typename T>
    constexpr explicit ScopedLayers(const std::array<T, N>& defArray)
        : layers(std::apply([](const auto&... defs) {
              return std::array{ Layer::load(defs)... };
          }, defArray)) {}

    ~ScopedLayers() {
        for (const auto& layer : layers) {
            UnloadTexture(layer.texture);
        }
    }

    ScopedLayers(const ScopedLayers&) = delete;
    ScopedLayers& operator=(const ScopedLayers&) = delete;
    ScopedLayers(ScopedLayers&&) = delete;
    ScopedLayers& operator=(ScopedLayers&&) = delete;

    [[nodiscard]] auto begin() const { return layers.begin(); }
    [[nodiscard]] auto end() const { return layers.end(); }
};