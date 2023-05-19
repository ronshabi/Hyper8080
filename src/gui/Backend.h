#pragma once

#include <SDL2/SDL.h>

#include "Window.h"
#include <stdexcept>

namespace GUI {
class Backend {
public:
    explicit Backend();
    ~Backend();
};
} // namespace GUI
