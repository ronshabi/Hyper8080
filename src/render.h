#ifndef HYPER8080_RENDER_H
#define HYPER8080_RENDER_H

#include "defs.h"

void render_init(void);
void render_create_window(SDL_Window **,
			  SDL_Renderer **,const char *,
    int w, int h, float scale);
void render_destroy(SDL_Window **win, SDL_Renderer **ren);
void render_update(SDL_Renderer **ren);
void render_clear(SDL_Renderer **ren);
void render_set_pixel(SDL_Renderer **ren, int x, int y);
void render(cpu *c, int vramOffset, SDL_Renderer **ren);

#endif /* HYPER8080_RENDER_H */