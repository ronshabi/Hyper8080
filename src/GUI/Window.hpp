#pragma once

#include <Core/Types.hpp>
#include <Rendering/Common.hpp>

#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <spdlog/spdlog.h>
#include <string>
#include <stdexcept>
#include <memory>
#include <thread>
#include <cstring>
#include <atomic>
#include <cstdlib>

namespace GUI {
class Window {
public:
    explicit Window(std::string&& title);
    ~Window();
    void Stop();
    bool isRunning() const;
private:

    void ThreadLoop();
    void Render();
    void Update();
    void HandleInputs();

    std::string m_title;
    const int m_width;
    const int m_height;
    std::atomic<bool> m_isRunning{ true };

    SDL_Window* m_windowPtr { nullptr };
    SDL_Surface* m_windowSurfacePtr { nullptr };

    std::mutex m_mutex{};
    std::thread m_thread{};
};
} // namespace GUI