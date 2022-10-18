#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	C_Init (&c);

	uint8_t *memory = calloc (0xffff, 1);
	C_SetMemory (&c, memory);

	free (memory);
	return 1;
}