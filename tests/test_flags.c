#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.h"
#include "../src/cpu.c"
int main (void)
{
	cpu c;
	cpu_init (&c);

	uint8_t *memory = malloc (2000);
	cpu_set_memory (&c, memory);

	c.a = 0x22;
	c.b = 0xdf;

	// Addition should be 0x101
	// Subtraction should be -0xbd

	uint8_t result = c.a + c.b;

	cpu_set_flags_all(&c, c.a, c.b, 0);

	if (c.flag_z == 0 && c.flag_s == 0 && c.flag_p == 0 && // Parity Odd
		c.flag_c == 1 && c.flag_ac == 0)
	{
		return 0;
	}

	return 1;
}