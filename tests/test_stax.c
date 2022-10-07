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

	C_SetBC (&c, 0x3333);
	C_SetByte (&c, 0x3333, 0x12);
	c.a = 0x75;

	stax_b (&c);

	int passed = 0;

	if (cpu_get_byte (&c, 0x3333) == 0x75) { passed++; }

	C_SetDE (&c, 0x4545);
	C_SetByte (&c, 0x4545, 0xea);
	c.a = 0xb2;

	stax_d (&c);

	if (cpu_get_byte (&c, 0x4545) == 0xb2) { passed++; }

	if (passed == 2) { return 0; }

	free (memory);
	return 1;
}