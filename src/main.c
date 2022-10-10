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
		// Get user input
		Sys_PollEvents (&c, &e, &quit);

		// Get time
		now = Sys_UnixTime ();

		//
		// Emulate
		// 2mhz -> 2 million cycles per second -> -> 2 cycles per 1000 ns

#ifdef DEBUG_MODE_REGULAR
		C_DisAsm (&c);
#endif
		c_currentOpcode = C_GetByte (&c, c.pc);
		C_Emulate (&c, c_currentOpcode);
#ifdef DEBUG_MODE_REGULAR
		printf ("\n");
#endif

		// Send interrupts @ 60hz
		delta_interruptTime = now - lastInterrupt;
		if (delta_interruptTime > HZ (120))
		{
			if (c.interrupts_enabled)
			{
				if (interrupt == 1) { R_Render (&c, 0x2400, &Renderer); }
				C_GenerateInterrupt (&c, interrupt + 1);
				lastInterrupt = now;
				interrupt	  = 1 - interrupt;
			}
		}
	}

	R_Exit (&Window, &Renderer);
	free (buffer);

	return 0;
}
