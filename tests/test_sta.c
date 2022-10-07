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

	c.pc = 0;
	C_SetWord (&c, 0x1, 0xcafe);
	C_SetByte (&c, 0xcafe, 0x12);
	c.a = 0xfe;

	sta (&c);

	if (cpu_get_byte (&c, 0xcafe) == 0xfe) { return 0; }

	return 1;
}