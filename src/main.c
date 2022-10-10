#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define HZ(v) (1000000000 / v)
#define FPS	  20

#include "defs.h"

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

	FILE		  *f;
	cpu			   c;
	unsigned char *buffer;
	uint8_t		   c_currentOpcode;
	SDL_Event	   e;
	bool		   quit		= false;
	SDL_Window	  *Window	= NULL;
	SDL_Renderer  *Renderer = NULL;
	long		   now = 0, lastInterrupt = 0, delta_interruptTime = 0;
	int			   interrupt	   = 0;
	long		   c_LastExecution = 0;
	long		   c_CycleCount	   = 0;
	long		   c_CycleLast	   = 0;
	long		   c_CycleDelta	   = 0;
	long		   c_Delta		   = 0;

	Sys_AllocateMemory (&buffer, 0xffff);
	Sys_LoadROM (f, argv[1], buffer);

	C_Init (&c);
	c.memory = buffer;

	//
	// Create window
	//
	R_Init ();
	R_CreateWindow (&Window, &Renderer, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_SCALE);
	R_ClearScreen (&Renderer);
	R_Update (&Renderer);
	R_SetColorWhite (&Renderer);

	//
	// Event loop
	//
	while (!quit && !c.halt)
	{

		now			 = SDL_GetTicks ();
		c_CycleCount = c.cycles;
		c_CycleDelta = c_CycleCount - c_CycleLast;
		c_Delta		 = now - c_LastExecution;
		Sys_PollEvents (&c, &e, &quit);

		//
		// Emulate
		// 2mhz -> 2 million cycles per second
		if (c_Delta > 2000)
		{
			printf ("Cycles: %lu\n", SDL_GetTicks ());

			// sleep based on cycle delta
			if (c_CycleDelta > 2000)
			{
				//				long			howmuchsleep = c_CycleDelta - 10000;
				//				struct timespec sleepts;
				//				sleepts.tv_sec	= 0;
				//				sleepts.tv_nsec = howmuchsleep * 1000;
				//				nanosleep (&sleepts, NULL);
				c_CycleLast = c_CycleCount;
			}
			c_LastExecution = now;
		}

		// Send interrupts @ 60hz
		delta_interruptTime = now - lastInterrupt;
		if (delta_interruptTime > 1000 / 120)
		{
			if (c.interrupts_enabled)
			{
				if (interrupt == 1) { R_Render (&c, 0x2400, &Renderer); }
				C_GenerateInterrupt (&c, interrupt + 1);
				lastInterrupt = now;
				interrupt	  = 1 - interrupt;
			}
		}

		// C_DisAsm (&c);
		c_currentOpcode = C_GetByte (&c, c.pc);
		c.i0			= 0b00001110;
		c.i1			= 0b10001101;
		c.i2			= 0b10001000;
		C_Emulate (&c, c_currentOpcode);
		// printf ("\n");
	}

	R_Exit (&Window, &Renderer);
	free (buffer);

	return 0;
}
