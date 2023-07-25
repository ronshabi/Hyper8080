#include "Backend.hpp"

namespace GUI {
Backend::Backend()
{
    // Init SDL
    auto sdl_init_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdl_init_result != 0)
        throw std::runtime_error("SDL backend could not initialize");

    // Set SDL texture filtering mode to linear
    auto sdl_texture_filtering_linear_result = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // Check if filtering mode really changed to linear
    if (sdl_texture_filtering_linear_result == false)
        throw std::runtime_error("SDL could not set texture filtering mode to linear");


}
Backend::~Backend() { SDL_Quit(); }
} // namespace GUI