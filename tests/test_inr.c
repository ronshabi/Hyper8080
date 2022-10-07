#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	cpu_init (&c);

	uint8_t *memory = calloc (10000, 1);
	C_SetMemory (&c, memory);

	inr (&c, &(c.b));
	inr (&c, &(c.b));
	inr (&c, &(c.b));
	C_SetHL (&c, 0x1234);
	C_SetByte (&c, 0x1234, 0xfa);
	inr_m (&c);

	if (c.b == 3 && cpu_get_byte (&c, 0x1234) == 0xfb && !c.flag_z && c.flag_s && !c.flag_p) { return 0; }

	free (memory);
	return 1;
}