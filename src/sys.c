#include "sys.h"

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
					case SDLK_q: *quit = true; break; // Quit

					case SDLK_RETURN:
						c->i1 |= 1;
						println ("+1 Coin");
						break; // Coin
					case SDLK_1:
						c->i1 |= 0x4;
						println ("Player 1 Start");
						break; // P1 Start
					case SDLK_SPACE:
						c->i1 |= 0x10;
						println ("Player 1 Fire");
						break; // Fire
					case SDLK_LEFT:
						c->i1 |= 0x20;
						println ("Player 1 Left");
						break; // P1 Left
					case SDLK_RIGHT:
						c->i1 |= 0x40;
						println ("Player 1 Right");
						break; // P1 Right
				}
			}
			case SDL_KEYUP: {
				switch (e->key.keysym.sym)
				{
					case SDLK_RETURN: c->i1 &= ~0x1; break; // Coin
					case SDLK_1: c->i1 &= ~0x4; break;		// P1 Start
					case SDLK_SPACE: c->i1 &= ~0x10; break; // Fire
					case SDLK_LEFT: c->i1 &= ~0x20; break;	// P1 Left
					case SDLK_RIGHT: c->i1 &= ~0x40; break; // P1 Right
				}
			}
		}
	}
}
