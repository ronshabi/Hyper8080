#pragma once

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include "Window.hpp"


namespace GUI {
class Backend {
public:
    explicit Backend();
    ~Backend();
};
} // namespace GUI
