#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.h"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	C_Init (&c);

	uint8_t *memory = calloc (10000, 1);
	C_SetMemory (&c, memory);

	c.a	 = 0x56;
	c.pc = 0;
	C_SetByte (&c, 0x1, 0xbe);
	adi (&c);

	if (c.a == 0x14 && c.flag_c && c.flag_p && !c.flag_z && !c.flag_s) { return 0; }

	return 1;
}