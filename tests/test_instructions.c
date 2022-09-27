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

	// DAD B
	c.b = 0x33;
	c.c = 0x9f;
	c.h = 0xa1;
	c.l = 0x7b;
	dad_b (&c);
	if (cpu_get_hl (&c) == 0xD51A && c.flag_c == 0) { return 0; }

	// INX D
	c.d = 0x38;
	c.e = 0xff;

	inx_d (&c);

	if (cpu_get_de (&c) == 0x3900) { return 0; }

	// INX SP
	c.sp = 0xffff;
	inx_sp (&c);
	if (c.sp == 0) { return 0; }

	return 1;
}