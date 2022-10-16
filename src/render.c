#include "render.h"

// ====================================
//  WINDOW
// ====================================
void R_Init (void)
{
	if (SDL_Init (SDL_INIT_VIDEO) != 0) { R_SDLError ("Failed to init SDL"); }

	//	 Set texture filtering to linear
	if (!SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1")) { R_SDLError ("Failed to set linear texture filtering"); }
}

void R_CreateWindow (SDL_Window **win, SDL_Renderer **ren, const char *title, int w, int h, float scale)
{
	*win = SDL_CreateWindow (title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w * (int)scale, h * (int)scale, SDL_WINDOW_SHOWN);
	if (*win == NULL) { R_SDLError ("Failed to create window"); }

	*ren = SDL_CreateRenderer (*win, -1, SDL_RENDERER_ACCELERATED);
	if (*ren == NULL) { R_SDLError ("Failed to create renderer"); }

	// Set scale
	if (SDL_RenderSetScale (*ren, scale, scale) < 0) { R_SDLError ("Failed to set scale"); }
	if (SDL_RenderSetLogicalSize (*ren, w, h) < 0) { R_SDLError ("Failed to set logical renderer size"); }
}

void R_Exit (SDL_Window **win, SDL_Renderer **ren)
{
	SDL_DestroyWindow (*win);
	SDL_DestroyRenderer (*ren);
	*win = NULL;
	*ren = NULL;
	SDL_Quit ();
}

// ====================================
//  GRAPHICS
// ====================================
void R_Update (SDL_Renderer **ren) { SDL_RenderPresent (*ren); }

void R_ClearScreen (SDL_Renderer **ren)
{
	SDL_SetRenderDrawColor (*ren, 0, 0, 0, 0xff); // set to black
	SDL_RenderClear (*ren);
}

void R_CompletePixel (SDL_Renderer **ren, int r, int g, int b, int x, int y)
{
	// if (r > 0xff || g > 0xff || b > 0xff) { R_LogWarning ("Invalid pixel RGB values"); }

	SDL_SetRenderDrawColor (*ren, r & 0xff, g & 0xff, b & 0xff, 0xff);
	SDL_RenderDrawPoint (*ren, x, y);
}

void R_SetColorBlack (SDL_Renderer **ren) { SDL_SetRenderDrawColor (*ren, 0, 0, 0, 0xff); }
void R_SetColorWhite (SDL_Renderer **ren) { SDL_SetRenderDrawColor (*ren, 0xff, 0xff, 0xff, 0xff); }

void R_PlacePixel (SDL_Renderer **ren, int x, int y) { SDL_RenderDrawPoint (*ren, x, y); }

// ====================================
//  RENDERING
// ====================================
void R_Render (cpu *c, int vramOffset, SDL_Renderer **ren)
{
	R_ClearScreen (ren);
	R_SetColorWhite (ren);

	for (int x = 0; x < WINDOW_WIDTH; x++)
	{
		for (int y = 0; y < WINDOW_HEIGHT; y += 8)
		{

			uint8_t pixel = c->memory[vramOffset + (x * (WINDOW_HEIGHT / 8)) + (y / 8)];

			for (int bit = 0; bit < 8; bit++)
			{
				if ((pixel >> bit) & 1) {
					R_PlacePixel (ren, x, WINDOW_HEIGHT - y - bit);
				}
			}
		}
	}

	R_Update (ren);
}
