#pragma once

#include <Core/Types.hpp>
#include <Core/Rendering.hpp>

constexpr f32 FpsToHz(int fps) {
    return float(1000.0) / float(fps);
}