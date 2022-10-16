#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.h"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	C_Init (&c);

	uint8_t *memory = calloc (10000, 1);
	C_SetMemory (&c, memory);

	// C2
	c.pc	 = 0000;
	c.flag_c = 0;
	C_SetByte (&c, 0x1, 0x56);
	C_SetByte (&c, 0x3, 0xbe);
	mvi (&c, &(c.a));
	c.pc++;
	aci (&c);

	if (c.a == 0x14 && c.flag_c) { return 0; }

	free (memory);
	return 1;
}