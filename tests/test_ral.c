#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"
#include "../src/debug.h"
#include "../src/debug.c"

int main (void)
{
	cpu c;
	C_Init (&c);

	c.a = 0x0b5;

	ral (&c);

	if (c.a == 0x6a && c.flag_c) { return 0; }

	return 1;
}
