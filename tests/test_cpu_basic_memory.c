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

	c.a = 0x0a;
	c.b = 0x0b;
	c.c = 0x0c;
	c.d = 0xfa;
	c.e = 0xce;

	C_SetByte (&c, 0xcafe, 0xca);
	C_SetByte (&c, 0xface, 0xdf);
	C_SetByte (&c, 0x0b0c, 0x33);

	C_SetWord (&c, 0xfeed, 0xabcd);

	if (C_DerefBC (&c) != 0x33) return 1;

	C_SetBC (&c, 0xcafe);

	if (C_DerefBC (&c) != 0xca) return 1;

	if (C_DerefDE (&c) != 0xdf) return 1;

	return 0;
}