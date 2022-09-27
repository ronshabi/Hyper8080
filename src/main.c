#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

#include "cpu.h"
#include "debug.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file...>\n", argv[0]);
		exit(1);
	}

	FILE *f = fopen(argv[1], "rb");

	// Check if file exists
	if (f == NULL)
	{
		fprintf(stderr, "%s: ", argv[0]);
		fprintf(stderr, ANSI_COLOR_RED);
		fprintf(stderr, "fatal error: ");
		fprintf(stderr, ANSI_COLOR_RESET);
		fprintf(stderr, "Couldn't open file %s: file doesn't exist.\n",
				argv[1]);
		exit(1);
	}

	// Get file size and read it into mem buffer
	fseek(f, 0L, SEEK_END);
	long fsize = ftell(f);
	fseek (f, 0L, SEEK_SET);

	unsigned char *buffer = calloc (10000, 1); // TODO: Change to match the CPU parameter
	fread (buffer, fsize, 1, f);
	fclose (f);

	cpu c;
	cpu_init (&c);
	c.memory = buffer;

	uint8_t current_opcode;

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
