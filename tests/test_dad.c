#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	int passed = 0;
	cpu c;
	C_Init (&c);

	C_SetBC (&c, 0x339f);
	C_SetHL (&c, 0xa17b);

	dad_b (&c);

	if (C_GetHL (&c) == 0xd51a && !c.flag_c) { passed++; }

	C_SetHL (&c, 0x04f2);
	dad_h (&c);

	if (C_GetHL (&c) == 0x09e4 && !c.flag_c) { passed++; }

	if (passed == 2) { return 0; }

	return 1;
}