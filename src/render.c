#include "render.h"

// ====================================
//  WINDOW
// ====================================
void R_Init (void)
{
	if (SDL_Init (SDL_INIT_VIDEO) != 0) { R_SDLError ("Failed to init SDL"); }

	// Set texture filtering to linear
	if (!SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1")) { R_SDLError ("Failed to set linear texture filtering"); }
}

void R_CreateWindow (SDL_Window *win, SDL_Renderer *ren, const char *title, int w, int h, float scale)
{
	win = SDL_CreateWindow (title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w * (int)scale, h * (int)scale, SDL_WINDOW_SHOWN);
	if (win == NULL) { R_SDLError ("Failed to create window"); }

	ren = SDL_CreateRenderer (win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL) { R_SDLError ("Failed to create renderer"); }

	// Set scale
	if (SDL_RenderSetScale (ren, scale, scale) < 0) { R_SDLError ("Failed to set scale"); }
	if (SDL_RenderSetLogicalSize (ren, w, h) < 0) { R_SDLError ("Failed to set logical renderer size"); }
}

void R_Exit (SDL_Window *win, SDL_Renderer *ren)
{
	SDL_DestroyWindow (win);
	SDL_DestroyRenderer (ren);
	win = NULL;
	ren = NULL;
	SDL_Quit ();
}

// ====================================
//  GRAPHICS
// ====================================
void R_Update (SDL_Renderer *ren) { SDL_RenderPresent (ren); }

void R_ClearScreen (SDL_Renderer *ren)
{
	SDL_SetRenderDrawColor (ren, 0, 0, 0, 0xff); // set to black
	SDL_RenderClear (ren);
}

void R_RenderPixel (SDL_Renderer *ren, int r, int g, int b, int x, int y)
{
	if (r > 0xff || g > 0xff || b > 0xff) { R_LogWarning ("Invalid pixel RGB values"); }

	SDL_SetRenderDrawColor (ren, r, g, b, 0xff);
	SDL_RenderDrawPoint (ren, x, y);
}
