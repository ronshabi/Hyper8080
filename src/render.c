#include "render.h"

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
