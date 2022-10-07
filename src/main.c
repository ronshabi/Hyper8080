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
#define WINDOW_SCALE	 4

#include "cpu.h"
#include "debug.h"
#include "render.h"

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
	==========================================================================================
	SDL
	==========================================================================================
	 */

	SDL_Event	  e;
	bool		  quit	   = false;
	SDL_Window	 *Window   = NULL;
	SDL_Renderer *Renderer = NULL;

	// Init
	SDL_Init (SDL_INIT_VIDEO);

	Window =
		SDL_CreateWindow (WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE,
						  SDL_WINDOW_SHOWN);							  // Create window
	SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1");					  // Set texture filtering to linear
	Renderer = SDL_CreateRenderer (Window, -1, SDL_RENDERER_ACCELERATED); // Create renderer for window
	SDL_RenderSetScale (Renderer, WINDOW_SCALE, WINDOW_SCALE);			  // Set renderer scale
	SDL_RenderSetLogicalSize (Renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	R_ClearScreen (Renderer);
	R_Update (Renderer);
	// ==================== Event loop ====================

	while (!quit)
	{
		// ---------- POLL EVENTS ---------------
		while (SDL_PollEvent (&e) != 0)
		{
			// quit
			if (e.type == SDL_QUIT) { quit = true; }

			// keypress
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_1: {
						c.i1 |= 0b00000100; // INPUT 1 BIT 2 - P1 START BUTTON
						break;
					}
					case SDLK_SPACE: {
						c.i1 |= 0b00010000; // INPUT 1 BIT 4 - P1 SHOOT
						break;
					}
					case SDLK_LEFT: {
						c.i1 |= 0b00100000; // INPUT 1 BIT 5 - P1 LEFT
						break;
					}
					case SDLK_RIGHT: {
						c.i1 |= 0b01000000; // INPUT 1 BIT 6 - P1 RIGHT
						break;
					}
					default: break;
				}
			}
		} // ------------------------------------ (/poll_events)

		// Clear screen
		// SDL_RenderClear (Renderer);
	}

	// ====================================================

	// ********** Close **********
	SDL_DestroyWindow (Window);
	SDL_DestroyRenderer (Renderer);
	Renderer = NULL;
	Window	 = NULL;
	SDL_Quit ();
	exit (3); // FIXME: Remove after finishing SDL implementation
			  // ***************************

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
