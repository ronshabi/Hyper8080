#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

#define R_LogWarning(msg) fprintf (stderr, "[Render] WARNING: %s\n", msg)
#define R_SDLError(msg)                             \
	do {                                            \
		fprintf (stderr, "[SDL] ERROR: %s\n", msg); \
		exit (7);                                   \
	} while (0)

void R_Init (void);
void R_CreateWindow (SDL_Window *win, SDL_Renderer *ren, const char *title, int w, int h, float scale);
void R_Exit (SDL_Window *win, SDL_Renderer *ren);

void R_Update (SDL_Renderer *ren);
void R_ClearScreen (SDL_Renderer *ren);
void R_RenderPixel (SDL_Renderer *ren, int r, int g, int b, int x, int y);

#endif // RENDER_H
