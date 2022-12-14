#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"
#include "../src/debug.h"
#include "../src/debug.c"

int main (void)
{
	cpu c;
	cpu_init (&c);

	c.a = 0xf2;

	rrc (&c);

	if (c.a == 0x79 && c.flag_c == 0) { return 0; }

	return 1;
}
