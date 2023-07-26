#pragma once

#include <Core/Types.hpp>

#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <spdlog/spdlog.h>
#include <string>
#include <stdexcept>
#include <memory>
#include <thread>

namespace GUI {
class Window {
public:
    explicit Window(std::string&& title, int scale = 1);
    void SetPixelBuffer(u8* address, u32 size);
    ~Window();

private:
    void SetRendererColor(u8 r, u8 g, u8 b);
    void PlacePixel(int x, int y);
    void Render();
    void Blit();
    void Clear();

    u8* m_pixelBuffer{};
    u32 m_pixelBufferSize{};

    SDL_Event m_sdlEvent;
    bool m_isRunning{true};
    std::string m_title;
    const int m_width;
    const int m_height;
    const int m_scale{2};

    std::mutex m_mutex;
    SDL_Window* m_sdlWindowPtr{ nullptr };
    SDL_Renderer* m_sdlRendererPtr{ nullptr };
};
} // namespace GUI