#include "defines.h"

int
main(int argc, char *argv[])
{

	/* Check arguments */

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file...>\n", argv[0]);
		exit(1);
	}

	FILE *f;
	struct cpu c;
	unsigned char *buffer;
	uint8_t c_currentOpcode;
	bool quit = false;
	SDL_Window *Window = NULL;
	SDL_Renderer *Renderer = NULL;
	uint64_t now = 0;
	uint64_t ms_Interrupt_Last = 0;
	uint64_t ms_Input_Last = 0;
	uint64_t ms_ClockSpeedMeasure_Last = 0;
	const uint8_t *keyboard = SDL_GetKeyboardState(NULL);

	int interrupt = 1;

	uint64_t cyclesLast = 0;
	uint64_t cyclesNow = 0;

	sys_allocate(&buffer, 65535);
	sys_load_rom(f, argv[1], buffer);

	cpu_init(&c);
	c.memory = buffer;


	/* Create window */

	render_init();
	render_create_window(&Window, &Renderer, WINDOW_TITLE, WINDOW_WIDTH,
	    WINDOW_HEIGHT, WINDOW_SCALE);
	render_clear(&Renderer);
	render_update(&Renderer);


	/* Event loop */

	while (!quit) {

		now = SDL_GetTicks();

		if ((double)(now - ms_Interrupt_Last) > HZ(120)) {
			if (interrupt ^= 1) {
				render(&c, 0x2400, &Renderer);
				/* Send RST 2 */
				cpu_interrupt(&c, 2);
			} else {
				/* Send RST 1 */
				cpu_interrupt(&c, 1);
			}
			ms_Interrupt_Last = now;
		}

		if ((double)(now - ms_Input_Last) > HZ(30)) {
			sys_keyboard(&c, &keyboard, &quit);
			ms_Input_Last = now;
		}

		cyclesNow = c.cycles;
		if (now - ms_ClockSpeedMeasure_Last > 1000) {
			printf("Running @ %.2f MHz\n",
			    (cyclesNow - cyclesLast) / 10E6);
			cyclesLast = cyclesNow;
			ms_ClockSpeedMeasure_Last = now;
		}

		if (!c.paused) {
			debug_disassemble(&c);

			c_currentOpcode = cpu_get_byte(&c, c.pc);
			c.pc += 1;
			cpu_execute(&c, c_currentOpcode);

			debug_stop(&c, &quit);
			D_Newline;
		}
	}

	debug_summary(&c);

	render_destroy(&Window, &Renderer);
	free(buffer);
	return 0;
}
