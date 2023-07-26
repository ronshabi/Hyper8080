#include "Window.hpp"

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

    // If all is successful, run the window thread loop
    std::thread windowThreadLoop([&]() {
        u64 ticksStart {};
        u64 ticksEnd {};
        u64 ticksDelta {};

        while (m_isRunning) {
            ticksEnd = SDL_GetTicks();

            m_mutex.lock();
            while (SDL_PollEvent(&m_sdlEvent)) {
                if (m_sdlEvent.key.keysym.sym == SDLK_q) {
                    spdlog::debug("Window thread of '{}': Quit requested", m_title);
                    m_isRunning = false;
                }
            }
            m_mutex.unlock();

            Clear();

            if (m_pixelBuffer != 0) {
                Render();
            }
            Blit();
            //            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            //            spdlog::debug("ticksDelta = {}", ticksDelta);

            ticksDelta = ticksEnd - ticksStart;
            if (ticksDelta > 1000) {
                ticksStart = SDL_GetTicks();
                spdlog::debug("Tick");
            }
        }
    });

    spdlog::debug("Window thread created for window '{}'", m_title);
    windowThreadLoop.join();
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

void Window::PlacePixel(const int x, const int y)
{
    SDL_RenderDrawPoint(m_sdlRendererPtr, x, y);
}
void Window::Blit()
{
    SDL_RenderPresent(m_sdlRendererPtr);
}
void Window::Clear()
{
    SDL_RenderClear(m_sdlRendererPtr);
}

void Window::Render()
{
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; x < m_height; y += 8) {
            u8 pixel = m_pixelBuffer[(x * m_height / 8) + y / 8];

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
void Window::SetRendererColor(const u8 r, const u8 g, const u8 b)
{
    SDL_SetRenderDrawColor(m_sdlRendererPtr, r, g, b, 0);
}

void Window::SetPixelBuffer(u8* address, u32 size)
{
    std::lock_guard lockGuard { m_mutex };
    m_pixelBuffer = address;
    m_pixelBufferSize = size;
    spdlog::debug("Window {}: set pixel buffer to {} (size {})", m_title, reinterpret_cast<void*>(m_pixelBuffer), m_pixelBufferSize);
}

} // namespace GUI