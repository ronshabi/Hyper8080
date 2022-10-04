#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	cpu_init (&c);
	int passed = 0;

	cpu_set_bc (&c, 0x00ff);
	cpu_set_de (&c, 0x0200);
	cpu_set_hl (&c, 0x0300);
	c.sp = 0x0001;
	inx_b (&c);
	inx_d (&c);
	inx_h (&c);
	inx_sp (&c);

	if (cpu_get_bc (&c) == 0x100 && cpu_get_de (&c) == 0x201 && cpu_get_hl (&c) == 0x301 && c.sp == 2) { return 0; }

	return 1;
}