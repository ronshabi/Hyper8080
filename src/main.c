#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define ANSI_COLOR_RED	 "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define WINDOW_WIDTH	 224
#define WINDOW_HEIGHT	 256
#define WINDOW_TITLE	 "8080 Emulator (unstable)"

#include "cpu.h"
#include "debug.h"

int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf (stderr, "Usage: %s <file...>\n", argv[0]);
		exit (1);
	}

	FILE *f = fopen (argv[1], "rb");

	// Check if file exists
	if (f == NULL)
	{
		fprintf (stderr, "%s: ", argv[0]);
		fprintf (stderr, ANSI_COLOR_RED);
		fprintf (stderr, "fatal error: ");
		fprintf (stderr, ANSI_COLOR_RESET);
		fprintf (stderr, "Couldn't open file %s: file doesn't exist.\n", argv[1]);
		exit (1);
	}

	// Get file size and read it into mem buffer
	fseek (f, 0L, SEEK_END);
	long fsize = ftell (f);
	fseek (f, 0L, SEEK_SET);

	unsigned char *buffer = calloc (10000, 1); // TODO: Change to match the CPU parameter
	fread (buffer, fsize, 1, f);
	fclose (f);

	cpu c;
	cpu_init (&c);
	c.memory = buffer;

	uint8_t current_opcode;

	/*
	==============================
	SDL
	==============================
	 */

	bool	  quit = false;
	SDL_Event e;

	// Init & create window
	SDL_Init (SDL_INIT_VIDEO);
	SDL_Window *WinMain =
		SDL_CreateWindow (WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	// ==================== Event loop ====================

	while (!quit)
	{
		// poll events
		while (SDL_PollEvent (&e) != 0)
		{

			// quit
			if (e.type == SDL_QUIT)
			{
				exit (3); // FIXME: Remove after finishing SDL implementation
				quit = true;
			}

			// keypress
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_UP: printf ("u\n"); break;
					case SDLK_DOWN: printf ("d\n"); break;
					case SDLK_LEFT: printf ("l\n"); break;
					case SDLK_RIGHT: printf ("r\n"); break;
					default: break;
				}
			}
		}
	}

	// ====================================================

	// Close
	SDL_DestroyWindow (WinMain);
	WinMain = NULL;
	SDL_Quit ();

#ifdef DEBUG_MODE_REGULAR
	printf ("PC\t\tOpcode\tA |B |C |D |E |H |L |SP  |CP  |ZSPCA\tInstruction\targs\n");
	printf ("--\t\t------\t--|--|--|--|--|--|--|----|----|-----\t-----------\t----\n");
#endif

	// Emulation loop
	while (!c.halt)
	{
		current_opcode = cpu_get_byte (&c, c.pc);
		cpu_disasm (&c);
		cpu_emulate (&c, current_opcode);
#ifdef DEBUG_MODE_REGULAR
		printf ("\n");
#endif
#ifdef DEBUG_MODE_STOP
		if (c.instructions == DEBUG_MODE_STOP_AFTER_INSTRUCTION)
		{
			printf ("STOPPED AT %d\n", DEBUG_MODE_STOP_AFTER_INSTRUCTION);
			printf ("Instructions executed: %lu\n", c.instructions);
			printf ("Cycles: %lu\n", c.cycles);
			break;
		}
#endif
	}
	return 0;
}
