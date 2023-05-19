#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <stdexcept>

namespace GUI {
class Window {
public:
    explicit Window(std::string&& title, int scale = 1);
    ~Window();

private:
    std::string m_title;
    const int m_width;
    const int m_height;

    SDL_Window* m_sdl_window_ptr;
};
} // namespace GUI