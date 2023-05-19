#include "Backend.h"

namespace GUI {
Backend::Backend()
{
    // init SDL
    auto sdl_init_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdl_init_result != 0)
        throw std::runtime_error("SDL backend could not initialize");

    // set SDL texture filtering mode to linear
    auto sdl_texture_filtering_linear_result
        = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    if (sdl_texture_filtering_linear_result == false)
        throw std::runtime_error(
            "SDL could not set texture filtering mode to linear");
}
Backend::~Backend() { SDL_Quit(); }
} // namespace GUI