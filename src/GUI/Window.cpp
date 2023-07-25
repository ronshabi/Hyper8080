#include "Window.hpp"
#include <SDL_render.h>
#include <stdexcept>

namespace GUI {
Window::Window(std::string&& title, int scale)
    : m_title { title }
    , m_width { 224 * scale }
    , m_height { 256 * scale }
{
    // Create window
    m_sdlWindowPtr = SDL_CreateWindow(m_title.c_str(), 0, 0, m_width, m_height, SDL_WINDOW_OPENGL);
    if (m_sdlWindowPtr == nullptr)
        throw std::runtime_error("Could not create SDL window");

    spdlog::debug("Created window '{}' ({}x{}) @ {}", m_title, m_width, m_height, reinterpret_cast<void*>(m_sdlWindowPtr));

    // Create renderer
    m_sdlRendererPtr = SDL_CreateRenderer(m_sdlWindowPtr, -1, SDL_RENDERER_ACCELERATED);
    if (m_sdlRendererPtr == nullptr)
        throw std::runtime_error("Could not create SDL window");

    spdlog::debug("Created renderer of window '{}' @ {}", m_title, reinterpret_cast<void*>(m_sdlRendererPtr));

    if (SDL_RenderSetScale(m_sdlRendererPtr, m_scale, m_scale) < 0)
        throw std::runtime_error("Could not set renderer scale");

    if (SDL_RenderSetLogicalSize(m_sdlRendererPtr, m_width, m_height) < 0)
        throw std::runtime_error("Could not set renderer logical size");

}
Window::~Window()
{
    SDL_DestroyRenderer(m_sdlRendererPtr);
    m_sdlRendererPtr = nullptr;
    spdlog::debug("Destroyed renderer of window '{}'", m_title);

    SDL_DestroyWindow(m_sdlWindowPtr);
    m_sdlWindowPtr = nullptr;
    spdlog::debug("Destroyed window '{}' ({}x{})", m_title, m_width, m_height);
}

void Window::PlacePixel(const int x, const int y) {
    SDL_RenderDrawPoint(m_sdlRendererPtr, x, y);
}
void Window::Blit() {
    SDL_RenderPresent(m_sdlRendererPtr);
}
void Window::Clear() {
    SDL_RenderClear(m_sdlRendererPtr);
}
void Window::RenderVRam(u8* vramPtr) {
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; x < m_height; y += 8) {
            u8 pixel = vramPtr[(x * m_height / 8) + y/8];

            for (u8 bit = 0; bit < 8; bit++) {
                if (y < m_height - 32 && y > m_width - 64) {
                    SetRendererColor(0xff, 0x00, 0x00); // Red
                } else if (y < 75 && y > 32) {
                    SetRendererColor(0x00, 0xff, 0x00); // Green
                } else {
                    SetRendererColor(0xff, 0xff, 0xff); // White
                }

                if ((pixel >> bit) & 1) {
                    PlacePixel(x, m_height - y - bit);
                } else {
                    SetRendererColor(0x00, 0x00, 0x00); // Black
                }
            }
        }
    }
}
void Window::SetRendererColor(const u8 r, const u8 g, const u8 b) {
    SDL_SetRenderDrawColor(m_sdlRendererPtr, r, g, b, 0);
}

} // namespace GUI