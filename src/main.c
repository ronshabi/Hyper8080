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
	bool		   quit				  = false;
	SDL_Window	  *Window			  = NULL;
	SDL_Renderer  *Renderer			  = NULL;
	uint64_t	   now				  = 0;
	uint64_t	   ms_Interrupt_Last  = 0;
	uint64_t	   ms_Input_Last			 = 0;
	uint64_t	   ms_ClockSpeedMeasure_Last = 0;
	const uint8_t *keyboard					 = SDL_GetKeyboardState (NULL);

	int interrupt = 1;

	uint64_t cyclesLast = 0;
	uint64_t cyclesNow	= 0;

	Sys_AllocateMemory (&buffer, 65535);
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

	//
	// Event loop
	//
	while (!quit)
	{

		now = SDL_GetTicks ();

		if ((double)(now - ms_Interrupt_Last) > HZ (120))
		{
			if (interrupt ^= 1)
			{
				R_Render (&c, 0x2400, &Renderer);
				// send RST 2
				C_GenerateInterrupt (&c, 0x10);
			}
			else
			{
				// send RST 1
				C_GenerateInterrupt (&c, 0x08);
			}
			ms_Interrupt_Last = now;
		}

		if ((double)(now - ms_Input_Last) > HZ (30))
		{
			// Reset inputs instead of handling KEYUP events
			c.i0 &= 0b10001111;
			c.i1 &= 0b10001000;
			c.i2 &= 0b10001011;

			SDL_PumpEvents ();

			if (keyboard[SDL_SCANCODE_Q]) { quit = true; }	 // QUIT
			if (keyboard[SDL_SCANCODE_C]) { c.i1 |= 1; }	 // INSERT COIN
			if (keyboard[SDL_SCANCODE_Z]) { c.paused ^= 1; } // PAUSE (for debugging, not implemented in the 8080 itself -r.s.)

			if (keyboard[SDL_SCANCODE_1]) { c.i1 |= 0x4; } // P1 Start
			if (keyboard[SDL_SCANCODE_1]) { c.i1 |= 0x2; } // P2 Start

			if (keyboard[SDL_SCANCODE_SPACE]) // Player 1/2 Shoot
			{
				c.i0 |= 0x10;
				c.i1 |= 0x10;
				c.i2 |= 0x10;
			}

			if (keyboard[SDL_SCANCODE_LEFT]) // Player 1/2 Left
			{
				c.i0 |= 0x20;
				c.i1 |= 0x20;
				c.i2 |= 0x20;
			}

			if (keyboard[SDL_SCANCODE_RIGHT]) // Player 1/2 Right
			{
				c.i0 |= 0x40;
				c.i1 |= 0x40;
				c.i2 |= 0x40;
			}

			ms_Input_Last = now;
		}

		cyclesNow = c.cycles;

		if (now - ms_ClockSpeedMeasure_Last > 1000)
		{
			printf ("Running @ %.2f mhz\n", (cyclesNow - cyclesLast) / 10E6);
			cyclesLast				  = cyclesNow;
			ms_ClockSpeedMeasure_Last = now;
		}

		if (!c.paused)
		{
#ifdef DEBUG_MODE_REGULAR
			C_DisAsm (&c);
#endif
			c_currentOpcode = C_GetByte (&c, c.pc);
			c.pc += 1;

#ifdef DEBUG_MODE_STOP
			if (c.instructions == DEBUG_MODE_STOP_AT_INSTRUCTION) { quit = true; }
#endif
			C_Emulate (&c, c_currentOpcode);

#ifdef DEBUG_MODE_REGULAR
			printf ("\n");
#endif
		}

	} // End of Game loop

	R_Exit (&Window, &Renderer);
	free (buffer);
#ifdef DEBUG_MODE_STOP
	printf ("Stopped at instruction %lu\n", c.instructions);
	printf ("Cycles: %lu\n", c.cycles);
#endif
	return 0;
}
