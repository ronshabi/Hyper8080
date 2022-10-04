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

	int passed = 0;

	cpu_set_word (&c, 1, 0xcafe);
	c.pc = 0;
	lxi_b (&c);

	if (c.b == 0xca && c.c == 0xfe) { passed++; }

	cpu_set_word (&c, 1, 0xefac);
	c.pc = 0;

	lxi_d (&c);

	if (cpu_get_de (&c) == 0xefac) { passed++; }

	cpu_set_word (&c, 1, 0xeb12);
	c.pc = 0;

	lxi_h (&c);

	if (cpu_get_hl (&c) == 0xeb12) { passed++; }

	cpu_set_word (&c, 1, 0x1f8e);
	c.pc = 0;

	lxi_sp (&c);

	if (c.sp == 0x1f8e) { passed++; }

	free (memory);
	if (passed == 4) { return 0; }
	return 1;
}