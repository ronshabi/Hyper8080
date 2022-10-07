#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.h"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	cpu_init (&c);

	uint8_t *memory = malloc (10000);
	C_SetMemory (&c, memory);
	stack_push (&c, 0xcafe);

	if (stack_pop (&c) == 0xcafe) { return 0; }

	return 1;
}