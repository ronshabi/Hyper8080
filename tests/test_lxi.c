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

	int passed = 0;

	C_SetWord (&c, 1, 0xcafe);
	lxi_b (&c);
	if (c.b == 0xca && c.c == 0xfe) { passed++; }

	C_SetWord (&c, 1, 0xefac);
	lxi_d (&c);

	if (C_GetDE (&c) == 0xefac) { passed++; }

	C_SetWord (&c, 1, 0xeb12);
	c.pc = 0;

	lxi_h (&c);
	c.pc++;

	if (C_GetHL (&c) == 0xeb12) { passed++; }

	C_SetWord (&c, 1, 0x1f8e);
	c.pc = 0;

	lxi_sp (&c);
	c.pc++;

	if (c.sp == 0x1f8e) { passed++; }

	free (memory);
	if (passed == 4) { return 0; }
	return 1;
}