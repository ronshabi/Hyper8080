#include "defs.h"
#include <sys/time.h>

uint64_t UnixMS ()
{
	struct timeval tv;
	gettimeofday (&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

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
	bool		   quit				  = false;
	SDL_Window	  *Window			  = NULL;
	SDL_Renderer  *Renderer			  = NULL;
	uint64_t	   now				  = 0;
	uint64_t	   ms_Interrupt_Last  = 0;
	uint64_t	   ms_Input_Last	  = 0;
	uint64_t	   ms_DebugPrint_Last = 0;
	const uint8_t *keyboard			  = SDL_GetKeyboardState (NULL);

	int interrupt = 1;

	uint64_t cyclesLast = 0;
	uint64_t cyclesNow	= 0;

	uint64_t cpuLastSlowdown = 0;

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

		now		  = UnixMS ();
		cyclesNow = c.cycles;

		if ((double)(now - ms_Interrupt_Last) > (double)(1000.0 / 120.0))
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
			cyclesLast		  = cyclesNow;
		}

		if ((double)(now - ms_Input_Last) > (double)(1000.0 / 30.0))
		{
			c.i0 &= 0b10001111;
			c.i1 &= 0b10001000;
			c.i2 &= 0b10001011;
			// Sys_PollEvents (&c, &e, &quit);

			SDL_PumpEvents ();

			if (keyboard[SDL_SCANCODE_Q])
			{
				println ("I wanna quit!");
				quit = true;
			}

			if (keyboard[SDL_SCANCODE_C]) { c.i1 |= 1; }
			if (keyboard[SDL_SCANCODE_Z]) { c.paused ^= 1; }

			if (keyboard[SDL_SCANCODE_1]) { c.i1 |= 0x4; }

			if (keyboard[SDL_SCANCODE_SPACE])
			{
				c.i0 |= 0x10;
				c.i1 |= 0x10;
				c.i2 |= 0x10;
			}

			if (keyboard[SDL_SCANCODE_LEFT])
			{
				c.i0 |= 0x20;
				c.i1 |= 0x20;
				c.i2 |= 0x20;
			}

			if (keyboard[SDL_SCANCODE_RIGHT])
			{
				c.i0 |= 0x40;
				c.i1 |= 0x40;
				c.i2 |= 0x40;
			}

			ms_Input_Last = now;
		}

		//		if ((double)(now - ms_DebugPrint_Last) > (double)(1000))
		//		{
		//			printf ("%f mhz\n", ((cyclesNow - cyclesLast) / 1000000.0));
		//
		//			// if (cyclesNow - cyclesLast > 2000000) { c_currentOpcode = 0; }
		//			// else { c.halt = 0; }
		//			ms_DebugPrint_Last = now;
		//		}

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
			for (int i = 0; i < 2000; i++) {}
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
