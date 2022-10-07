#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define ANSI_COLOR_RED	 "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define WINDOW_WIDTH	 224
#define WINDOW_HEIGHT	 256
#define WINDOW_TITLE	 "8080 Emulator (unstable)"
#define WINDOW_SCALE	 1.0F

#include "cpu.h"
#include "debug.h"
#include "render.h"

double Sys_Time (void)
{
	struct timespec ts;
	static int		secbase;

	clock_gettime (CLOCK_REALTIME, &ts);

	if (!secbase)
	{
		secbase = ts.tv_sec;
		return ts.tv_nsec / 1000000.0;
	}

	return (ts.tv_sec - secbase) + ts.tv_nsec / 1000000.0;
}

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

	unsigned char *buffer = calloc (0xffff, 1); // TODO: Change to match the CPU parameter
	fread (buffer, fsize, 1, f);
	fclose (f);

	cpu c;
	C_Init (&c);
	c.memory = buffer;

	uint8_t current_opcode;

	/*
	==========================================================================================
	SDL
	==========================================================================================
	 */

	double		  lastTimer		 = 0;
	double		  nextInterrupt	 = 0;
	int			  whichInterrupt = 0;
	SDL_Event	  e;
	bool		  quit	   = false;
	SDL_Window	 *Window   = NULL;
	SDL_Renderer *Renderer = NULL;

	// Create window
	R_Init ();
	R_CreateWindow (Window, Renderer, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_SCALE);
	R_ClearScreen (Renderer);
	R_Update (Renderer);

	// ==================== Event loop ====================
	while (!quit)
	{
		//
		// Poll Events
		//
		while (SDL_PollEvent (&e) != 0)
		{
			// quit
			if (e.type == SDL_QUIT) { quit = true; }

			// Key down
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_RETURN: {
						printf ("Coin inserted\n");
						c.i1 &= ~(0b00000001); // Coin
						break;
					}
					case SDLK_1: {
						c.i1 |= 0b00000100; // P1 Start
						break;
					}
					case SDLK_SPACE: {
						c.i1 |= 0b00010000; // P1 Shoot
						break;
					}
					case SDLK_LEFT: {
						c.i1 |= 0b00100000; // P1 Left
						break;
					}
					case SDLK_RIGHT: {
						c.i1 |= 0b01000000; // P1 Right
						break;
					}
					default: break;
				}
			}
			// Key up
			else if (e.type == SDL_KEYUP)
			{
				switch (e.key.keysym.sym)
				{
						//					case SDLK_RETURN: {
						//						c.i1 &= (0b00000001); // Coin
						//						break;
						//					}
					case SDLK_1: {
						c.i1 &= ~(0b00000100); // P1 Start
						break;
					}
					case SDLK_SPACE: {
						break;
					}
					case SDLK_LEFT: {
						c.i1 |= ~(0b00100000); // P1 Left
						break;
					}
					case SDLK_RIGHT: {
						c.i1 |= ~(0b01000000); // P1 Right
						break;
					}
					default: break;
				}
			}
		}

		//
		// If CPU not halted
		//
		while (!c.halt)
		{
			current_opcode = C_GetByte (&c, c.pc);
			C_DisAsm (&c);
			C_Emulate (&c, current_opcode);

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

			//
			// Time loop
			//
			if (Sys_Time () - nextInterrupt > (1.0 / 60.0))
			{

				// interrupt only if EI
				if (c.interrupts_enabled)
				{
					C_GenerateInterrupt (&c, 2);
					nextInterrupt = Sys_Time ();
					// printf ("%f\n", Sys_Time ());
				}

			} // 1/60sec has elapsed
		}
	}

	// ====================================================

	R_Exit (Window, Renderer);
	free (buffer);

	return 0;
}
