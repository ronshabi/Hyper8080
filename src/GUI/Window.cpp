#include "Window.hpp"
#include <spdlog/spdlog.h>

namespace GUI {
Window::Window(std::string&& title, int width, int height)
    : m_title { title }
    , m_width { width }
    , m_height { height }
{
    // Create window
    m_windowPtr = SDL_CreateWindow(m_title.c_str(), 0, 0, m_width, m_height, SDL_WINDOW_OPENGL);
    if (m_windowPtr == nullptr)
        throw std::runtime_error("Could not create SDL window");

    m_windowSurfacePtr = SDL_GetWindowSurface(m_windowPtr);
    if (m_windowSurfacePtr == nullptr)
        throw std::runtime_error("Could not get surface from SDL window");

    spdlog::debug("Created window '{}' ({}x{}) @ {} [Surface @ {}]", m_title, m_width, m_height, reinterpret_cast<void*>(m_windowPtr), reinterpret_cast<void*>(m_windowSurfacePtr));

    m_thread = std::move(std::thread(&Window::ThreadLoop, this));
}
Window::~Window()
{
    if (m_thread.joinable()) {
        m_thread.join();
    }
    SDL_DestroyWindow(m_windowPtr);
    m_windowPtr = nullptr;
    m_windowSurfacePtr = nullptr;
    spdlog::debug("Destroyed window '{}' ({}x{})", m_title, m_width, m_height);
}

void Window::Update()
{
    if (SDL_UpdateWindowSurface(m_windowPtr) != 0) {
        spdlog::error("Updating window surface failed for window {}, surface {}", reinterpret_cast<void*>(m_windowPtr), reinterpret_cast<void*>(m_windowSurfacePtr));
        m_isRunning = false;
    }
}

void Window::Render()
{

    //    std::memset(m_windowSurfacePtr->pixels, 0, m_windowSurfacePtr->pitch * m_height);

    if (m_drawFromBitmapSource) {
        for (u32 x = 0; x < m_width; x++) {
            for (u32 y = 0; y < m_height; y += 8) {
                u8 pixel = m_bitmapSourcePtr[x * (m_height / 8) + (y / 8)];

                for (int bit = 0; bit < 8; bit++) {
                    if (pixel & (1 << bit)) {
                        spdlog::info("true");
                        reinterpret_cast<u32*>(m_windowSurfacePtr->pixels)[(((u32)(m_height - y - bit)q) * m_width) + x] = 0x11223344;
                    } else {
//                        reinterpret_cast<u32*>(m_windowSurfacePtr->pixels)[(m_height - y - bit) * m_width + x] = 0x00ffffff;
                    }
                }
            }
        }
    }
}


//            if (m_drawFromBitmapSource) {
//                reinterpret_cast<u32*>(m_windowSurfacePtr->pixels)[y * m_width + x] = reinterpret_cast<u8*>(m_bitmapSourcePtr)[y * m_width + x]; ;
//            } else {
//                const int r = std::rand();
//                reinterpret_cast<u32*>(m_windowSurfacePtr->pixels)[y * m_width + x] = r;
//            }

void Window::HandleInputs()
{

}

void Window::ThreadLoop()
{
    spdlog::debug("Started thread for window '{}'", m_title);
    u64 ticksStart {};
    u64 ticksEnd {};
    u64 ticksDelta {};

    while (m_isRunning) {
        ticksEnd = SDL_GetTicks();

        HandleInputs();
        Render();

        ticksDelta = ticksEnd - ticksStart;
        if (ticksDelta > FpsToHz(60)) {
            m_mutex.lock();
            Update();
            ticksStart = SDL_GetTicks();
            m_mutex.unlock();
        }
    }
}

void Window::Stop() {
    spdlog::debug("Stopping window '{}' thread", m_title);
    m_isRunning = false;
}
bool Window::isRunning() const {
    return m_isRunning;
}
void Window::SetBitmapSource(u8* ptr) {
    m_bitmapSourcePtr = ptr;
}
void Window::setDrawFromBitmapSource(bool val) {
    m_drawFromBitmapSource = val;
}

} // namespace GUI