#include "Window.hpp"

namespace GUI {
Window::Window(std::string&& title)
    : m_title { title }
    , m_width { 224 * 2 }
    , m_height { 256 * 2 }
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
    // Render noise for now
    for (u32 y = 0; y < m_height; y++) {
        for (u32 x = 0; x < m_width; x++) {
            const int r = std::rand();
            reinterpret_cast<u32*>(m_windowSurfacePtr->pixels)[y * m_width + x] = r;
        }
    }
}

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

} // namespace GUI