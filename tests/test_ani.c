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

	c.a	 = 0x3a;
	c.pc = 0;
	cpu_set_byte (&c, 0x1, 0x0f);
	ani (&c);

	if (c.a == 0x0a) { return 0; }

	return 1;
}