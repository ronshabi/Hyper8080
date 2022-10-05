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

	cpu_set_byte (&c, 0x938b, 0x69);
	c.d = 0x93;
	c.e = 0x8b;

	ldax_d (&c);

	if (c.a == 0x69) { return 0; }

	free (memory);
	return 1;
}