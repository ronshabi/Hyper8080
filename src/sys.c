#include "sys.h"

long Sys_UnixTime (void)
{
	struct timespec ts;
	clock_gettime (CLOCK_REALTIME, &ts);
	return ts.tv_sec * 1E9 + ts.tv_nsec;
}

void Sys_LoadROM (FILE *fptr, char *filename, unsigned char *memptr)
{
	fptr = fopen (filename, "rb"); // (try to) open file

	// Check if file exists, if not -> exit and print error (with nice colors)
	if (fptr == NULL)
	{
		fprintf (stderr, ANSI_COLOR_RED);
		fprintf (stderr, "fatal error: ");
		fprintf (stderr, ANSI_COLOR_RESET);
		fprintf (stderr, "Couldn't open file %s: file doesn't exist.\n", filename);
		exit (1);
	}

	// get file size in bytes
	fseek (fptr, 0L, SEEK_END);
	long fsize = ftell (fptr);
	fseek (fptr, 0L, SEEK_SET);

	fread (memptr, fsize, 1, fptr); // Read to memory buffer
	fclose (fptr);					// File handle not needed anymore
}

void Sys_AllocateMemory (unsigned char **ptr, int howmuch) { *ptr = calloc (howmuch, 1); }

void Sys_MirrorRAM (unsigned char **ptr, uint16_t from, uint16_t howmuch)
{
	for (int i = from; i < (from + howmuch); i++) { (*ptr)[from + i] = (*ptr)[i]; }
}

void Sys_PollEvents (cpu *c, SDL_Event *e, bool *quit)
{
	while (SDL_PollEvent (e) != 0)
	{
		switch (e->type)
		{
			case SDL_QUIT: c->halt = true; break;
			case SDL_KEYDOWN: {
				switch (e->key.keysym.sym)
				{
					case SDLK_q: *quit = true; break;
					case SDLK_RETURN: c->i1 |= 1 << 0; break;
					case SDLK_1: c->i1 |= 1 << 2; break;
					case SDLK_2: c->i1 |= 1 << 1; break;
					case SDLK_DELETE: c->i2 |= 1 << 2; break;
					case SDLK_SPACE: {
						c->i0 |= 1 << 4;
						c->i1 |= 1 << 4;
						c->i2 |= 1 << 4;
						break;
					}
					case SDLK_LEFT: {
						c->i0 |= 1 << 5;
						c->i1 |= 1 << 5;
						c->i2 |= 1 << 5;
						break;
					}
					case SDLK_RIGHT: {
						c->i0 |= 1 << 6;
						c->i1 |= 1 << 6;
						c->i2 |= 1 << 6;
						break;
					}
				}
			}
			case SDL_KEYUP: {
				switch (e->key.keysym.sym)
				{
					case SDLK_SPACE: c->i1 &= 0b10001111; break; // P1 Shoot
					case SDLK_LEFT: c->i1 &= 0xdf; break;		 // P1 Left
					case SDLK_RIGHT: c->i1 &= 0xbf; break;		 // P1 Right
				}
			}
		}
	}
}
