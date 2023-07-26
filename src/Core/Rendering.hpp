#pragma once

#include <Core/Types.hpp>

constexpr u32 FpsToHz(int fps) {
    return u32(1000.0 / float(fps));
}