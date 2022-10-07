#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.h"
#include "../src/cpu.c"

#define ITER  10000
#define VALUE 0xaabbcc

int main (void)
{
	cpu c;
	cpu_init (&c);

	uint8_t *memory = malloc (2000);
	C_SetMemory (&c, memory);

	int i = 0;

	while (i < ITER)
	{
		stack_push (&c, (uint16_t)VALUE + i);
		i++;
	}

	i--;

	while (i != 0)
	{
		if (stack_pop (&c) != (uint16_t)VALUE + i) { return 1; }

		i--;
	}

	return 0;
}