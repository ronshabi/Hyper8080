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

	C_SetByte (&c, 0x938b, 0x69);
	c.d = 0x93;
	c.e = 0x8b;

	ldax_d (&c);

	if (c.a == 0x69) { return 0; }

	free (memory);
	return 1;
}