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

	c.pc = 0;
	cpu_set_word (&c, 0x1, 0xcafe);
	cpu_set_byte (&c, 0xcafe, 0x12);
	c.a = 0xfe;

	sta (&c);

	if (cpu_get_byte (&c, 0xcafe) == 0xfe) { return 0; }

	return 1;
}