#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"
#include "../src/debug.h"
#include "../src/debug.c"

int main (void)
{
	cpu c;
	C_Init (&c);

	c.h = 0x97;
	c.l = 0x00;

	dcx_h (&c);

	if (c.h = 0x97 && c.l == 0xff) { return 0; }

	return 1;
}
