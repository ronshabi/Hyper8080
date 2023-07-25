#pragma once

#include <Core/Types.hpp>

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <string>
#include <stdexcept>
#include <memory>

namespace GUI {
class Window {
public:
    explicit Window(std::string&& title, int scale = 1);
    ~Window();

    void RenderVRam(u8* vramPtr);
private:
    void SetRendererColor(u8 r, u8 g, u8 b);
    void PlacePixel(int x, int y);

    void Blit();
    void Clear();

    std::string m_title;
    const int m_width;
    const int m_height;
    const int m_scale{2};

    SDL_Window* m_sdlWindowPtr{ nullptr };
    SDL_Renderer* m_sdlRendererPtr{ nullptr };
};
} // namespace GUI