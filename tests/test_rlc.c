#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	C_Init (&c);

	c.a = 0Xf2;
	rlc (&c);

	if (c.a == 0xe5 && c.flag_c) { return 0; }

	return 1;
}