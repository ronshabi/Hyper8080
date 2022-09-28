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

	c.a	 = 0x0;
	c.pc = 0;
	cpu_set_byte (&c, 0x1, 0x1);
	sui (&c);
	if (c.a == 0xff && c.flag_c == 1) { return 0; }

	return 1;
}