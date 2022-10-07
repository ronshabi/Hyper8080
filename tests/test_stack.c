#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.h"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	C_Init (&c);

	uint8_t *memory = malloc (10000);
	C_SetMemory (&c, memory);
	S_Push (&c, 0xcafe);

	if (S_Pop (&c) == 0xcafe) { return 0; }

	return 1;
}