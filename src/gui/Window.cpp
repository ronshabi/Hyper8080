#include "Window.h"

namespace GUI {
Window::Window(std::string&& title, int scale)
    : m_title { title }
    , m_width { 224 * scale }
    , m_height { 256 * scale }
    , m_sdl_window_ptr { nullptr }
{
    SDL_CreateWindow(m_title.c_str(), 0, 0, m_width, m_height,
        SDL_WINDOWPOS_CENTERED | SDL_WINDOW_OPENGL | SDL_WINDOW_VULKAN);
}
Window::~Window()
{
    if (m_sdl_window_ptr != nullptr) {
        SDL_DestroyWindow(m_sdl_window_ptr);
    }
}
} // namespace GUI