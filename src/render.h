#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

#define R_LogWarning(msg) fprintf (stderr, "[Render] WARNING: %s\n", msg)

void R_Update (SDL_Renderer *ren);
void R_ClearScreen (SDL_Renderer *ren);
void R_RenderPixel (SDL_Renderer *ren, int r, int g, int b, int x, int y);

#endif // RENDER_H
