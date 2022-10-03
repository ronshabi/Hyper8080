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

	// C2
	c.pc	 = 0000;
	c.flag_c = 0;
	cpu_set_byte (&c, 0x1, 0x56);
	cpu_set_byte (&c, 0x3, 0xbe);
	mvi (&c, &(c.a));
	aci (&c);

	if (c.a == 0x14 && c.flag_c) { return 0; }

	free (memory);
	return 1;
}