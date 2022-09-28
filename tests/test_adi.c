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

	c.a	 = 0x14;
	c.pc = 0;
	cpu_set_byte (&c, 0x1, 0x42);
	adi (&c);

	if (c.a == 0x56) { return 0; }

	return 1;
}