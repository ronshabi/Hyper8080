#include "render.h"

#define RGB_BLACK 	0x00, 0x00, 0x00
#define RGB_WHITE 	0xff, 0xff, 0xff
#define RGB_GREEN	0x00, 0xff, 0x00
#define RGB_RED		0xff, 0x00, 0x00

void
render_init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Failed to init SDL");
		exit(1);
	}

	/*  Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		fprintf(stderr, "Failed to set linear texture filtering");
		exit(1);
	}
}

void
render_create_window(SDL_Window **win, SDL_Renderer **ren, const char *title, int w,
    int h, float scale)
{
	*win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
	    SDL_WINDOWPOS_UNDEFINED, w * (int)scale, h * (int)scale,
	    SDL_WINDOW_SHOWN);
	if (*win == NULL) {
		fprintf(stderr, "Failed to create window");
		exit(1);
	}

	*ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);
	if (*ren == NULL) {
		fprintf(stderr, "Failed to create renderer");
		exit(1);
	}

	/* Set scale */
	if (SDL_RenderSetScale(*ren, scale, scale) < 0) {
		fprintf(stderr, "Failed to set scale");
		exit(1);
	}
	if (SDL_RenderSetLogicalSize(*ren, w, h) < 0) {
		fprintf(stderr, "Failed to set logical renderer size");
		exit(1);
	}
}

void
render_destroy(SDL_Window **win, SDL_Renderer **ren)
{
	SDL_DestroyWindow(*win);
	SDL_DestroyRenderer(*ren);
	*win = NULL;
	*ren = NULL;
	SDL_Quit();
}

void
render_update(SDL_Renderer **ren)
{
	SDL_RenderPresent(*ren);
}

void
render_clear(SDL_Renderer **ren)
{
	SDL_RenderClear(*ren);
}

void
render_set_color_rgb(SDL_Renderer** ren, int r, int g, int b)
{
	SDL_SetRenderDrawColor(*ren, r, g, b, 00);
}

void
render_set_pixel(SDL_Renderer **ren, int x, int y)
{
	SDL_RenderDrawPoint(*ren, x, y);
}

void
render(cpu *c, int vramOffset, SDL_Renderer **ren)
{
	render_clear(ren);

	for (int x = 0; x < WINDOW_WIDTH; x++) {
		for (int y = 0; y < WINDOW_HEIGHT; y += 8) {

			uint8_t pixel = c->memory[vramOffset +
			    (x * (WINDOW_HEIGHT / 8)) + (y / 8)];

			for (int bit = 0; bit < 8; bit++) {
				if (WINDOW_HEIGHT - 32 > y &&
				    WINDOW_HEIGHT - 64 < y) {
					render_set_color_rgb(ren, RGB_RED);
				} else if (y < 75 && y > 32) {
					render_set_color_rgb(ren, RGB_GREEN);

				} else {
					render_set_color_rgb(ren, RGB_WHITE);
				}

				if ((pixel >> bit) & 1) {
					render_set_pixel(ren, x,
					    WINDOW_HEIGHT - y - bit);
				} else {
					render_set_color_rgb(ren, RGB_BLACK);
				}
			}
		}
	}

	render_update(ren);
}
