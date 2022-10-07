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
#define WINDOW_SCALE	 1

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

void Sys_LoadRomIntoMemory (FILE *fptr, char *filename, unsigned char *memptr)
{
	// (try to) open file
	fptr = fopen (filename, "rb");

	// Check if file exists, if not -> exit and print error (with nice colors)
	if (fptr == NULL)
	{
		fprintf (stderr, ANSI_COLOR_RED);
		fprintf (stderr, "fatal error: ");
		fprintf (stderr, ANSI_COLOR_RESET);
		fprintf (stderr, "Couldn't open file %s: file doesn't exist.\n", filename);
		exit (1);
	}

	// get size of file
	fseek (fptr, 0L, SEEK_END);
	long fsize = ftell (fptr);
	fseek (fptr, 0L, SEEK_SET);

	// Read to memory buffer
	fread (memptr, fsize, 1, fptr);

	// File handle not needed anymore
	fclose (fptr);
}

void Sys_Alloc (unsigned char **ptr, int howmuch) { *ptr = calloc (0xffff, 1); }

int main (int argc, char *argv[])
{
	//
	// Check arguments
	//
	if (argc != 2)
	{
		fprintf (stderr, "Usage: %s <file...>\n", argv[0]);
		exit (1);
	}

	FILE		  *romfile;
	cpu			   c;
	unsigned char *memory;

	Sys_Alloc (&memory, 0xffff);
	Sys_LoadRomIntoMemory (romfile, argv[1], memory);

	C_Init (&c);
	c.memory = memory;

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
	R_CreateWindow (&Window, &Renderer, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_SCALE);
	R_ClearScreen (&Renderer);
	R_Update (&Renderer);

	// ==================== Event loop ====================
	while (!quit)
	{
		//
		// Draw
		//
		for (int y = 0; y < WINDOW_HEIGHT; y++)
		{
			for (int x = 0; x < WINDOW_WIDTH; x++)
			{
				//				if (g > 255) {g = 0;}
				//				if (b > 255) {b = 0;}
				R_RenderPixel (&Renderer, x + (int)Sys_Time () & 0xff, y, ~(x + y), x, y);
			}
		}

		R_Update (&Renderer);

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
					case SDLK_q: {
						quit = true;
						break;
					}
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
		while (0)
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

	R_Exit (&Window, &Renderer);
	free (memory);

	return 0;
}
