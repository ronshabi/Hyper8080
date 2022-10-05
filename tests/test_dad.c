#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	int passed = 0;
	cpu c;
	cpu_init (&c);

	cpu_set_bc (&c, 0x339f);
	cpu_set_hl (&c, 0xa17b);

	dad_b (&c);

	if (cpu_get_hl (&c) == 0xd51a && !c.flag_c) { passed++; }

	cpu_set_hl (&c, 0x04f2);
	dad_h (&c);

	if (cpu_get_hl (&c) == 0x09e4 && !c.flag_c) { passed++; }

	if (passed == 2) { return 0; }

	return 1;
}