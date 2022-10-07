#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	cpu_init (&c);

	uint8_t *memory = calloc (10000, 1);
	C_SetMemory (&c, memory);

	C_SetByte (&c, 1, 0xca);
	c.pc = 0;
	mvi (&c, &(c.b));

	c.pc = 0;
	mvi_m (&c);

	free (memory);
	if (c.b == 0xca && C_DerefHL (&c) == 0xca) { return 0; }
	return 1;
}