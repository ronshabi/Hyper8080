#pragma once
#include "defs.h"

#define R_SDLError(msg)                             \
	do {                                            \
		fprintf (stderr, "[SDL] ERROR: %s\n", msg); \
		exit (7);                                   \
	} while (0)

void R_Init (void);
void R_CreateWindow (SDL_Window **win, SDL_Renderer **ren, const char *title, int w, int h, float scale);
void R_Exit (SDL_Window **win, SDL_Renderer **ren);
void R_Update (SDL_Renderer **ren);
void R_ClearScreen (SDL_Renderer **ren);
void R_SetColorBlack (SDL_Renderer **ren);
void R_SetColorWhite (SDL_Renderer **ren);
void R_PlacePixel (SDL_Renderer **ren, int x, int y);
void R_Render (cpu *c, int vramOffset, SDL_Renderer **ren);
