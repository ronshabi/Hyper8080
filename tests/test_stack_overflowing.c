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
	C_Init (&c);

	uint8_t *memory = malloc (2000);
	C_SetMemory (&c, memory);

	int i = 0;

	while (i < ITER)
	{
		S_Push (&c, (uint16_t)VALUE + i);
		i++;
	}

	i--;

	while (i != 0)
	{
		if (S_Pop (&c) != (uint16_t)VALUE + i) { return 1; }

		i--;
	}

	return 0;
}