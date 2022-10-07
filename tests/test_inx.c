#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	cpu_init (&c);
	int passed = 0;

	C_SetBC (&c, 0x00ff);
	C_SetDE (&c, 0x0200);
	C_SetHL (&c, 0x0300);
	c.sp = 0x0001;
	inx_b (&c);
	inx_d (&c);
	inx_h (&c);
	inx_sp (&c);

	if (C_GetBC (&c) == 0x100 && C_GetDE (&c) == 0x201 && C_GetHL (&c) == 0x301 && c.sp == 2) { return 0; }

	return 1;
}