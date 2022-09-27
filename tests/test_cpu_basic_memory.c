#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.h"
#include "../src/cpu.c"
int main (void)
{
	cpu c;
	cpu_init (&c);

	uint8_t *memory = calloc (10000, 1);
	cpu_set_memory (&c, memory);

	c.a = 0x0a;
	c.b = 0x0b;
	c.c = 0x0c;
	c.d = 0xfa;
	c.e = 0xce;

	cpu_set_byte(&c, 0xcafe, 0xca);
	cpu_set_byte(&c, 0xface, 0xdf);
	cpu_set_byte(&c, 0x0b0c, 0x33);

	cpu_set_word(&c, 0xfeed, 0xabcd);

	if (cpu_deref_bc(&c) != 0x33) return 1;

	cpu_set_bc(&c, 0xcafe);

	if (cpu_deref_bc(&c) != 0xca) return 1;

	if (cpu_deref_de(&c) != 0xdf) return 1;

	return 0;
}