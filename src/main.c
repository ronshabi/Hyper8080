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
	bool		   quit				  = false;
	SDL_Window	  *Window			  = NULL;
	SDL_Renderer  *Renderer			  = NULL;
	long		   now				  = 0;
	long		   ms_Interrupt_Last  = 0;
	long		   ms_Input_Last	  = 0;
	long		   ms_Interrupt_Delta = 0;
	const uint8_t *keyboard			  = SDL_GetKeyboardState (NULL);

	int interrupt = 0;

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

	//
	// Event loop
	//
	while (!quit && !c.halt)
	{

		now = SDL_GetTicks ();

		if ((now - ms_Interrupt_Last) > 1000 / 120 || ms_Interrupt_Last > now)
		{
			ms_Interrupt_Last = now;
			if (interrupt == 1)
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
			interrupt = 1 - interrupt;
		}

		if (now - ms_Input_Last > 1000 / 30 || ms_Input_Last > now)
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

			if (keyboard[SDL_SCANCODE_1]) { c.i1 |= 0x4; }

			if (keyboard[SDL_SCANCODE_SPACE])
			{
				c.i1 |= 0x10;
				c.i0 |= 0x10;
			}

			if (keyboard[SDL_SCANCODE_LEFT]) { c.i1 |= 0x20; }

			if (keyboard[SDL_SCANCODE_RIGHT]) { c.i1 |= 0x40; }

			ms_Input_Last = now;
		}

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

	R_Exit (&Window, &Renderer);
	free (buffer);
#ifdef DEBUG_MODE_STOP
	printf ("Stopped at instruction %lu\n", c.instructions);
	printf ("Cycles: %lu\n", c.cycles);
#endif
	return 0;
}
