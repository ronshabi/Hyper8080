#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	C_Init (&c);

	uint8_t *memory = calloc (10000, 1);
	C_SetMemory (&c, memory);

	c.h = 0x3a;
	c.l = 0x7c;

	C_SetByte (&c, 0x3a7c, 0x40);

	dcr_m (&c);

	if (C_GetByte (&c, 0x3a7c) == 0x3f) { return 0; }

	free (memory);
	return 1;
}