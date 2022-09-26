#include "../src/cpu.c"
#include "../src/cpu.h"
#include <stdlib.h>

#define ITER 10000
#define VALUE 0xaabbcc

int main(void)
{
	cpu c;
	cpu_init(&c);

	uint8_t *memory = malloc(2000);
	cpu_set_memory(&c, memory);

	int i = 0;

	while (i < ITER)
	{
		cpu_stack_push(&c, (uint16_t)VALUE + i);
		i++;
	}

	i--;

	while (i != 0)
	{
		if (cpu_stack_pop(&c) != (uint16_t)VALUE + i)
		{
			return 1;
		}

		i--;
	}

	return 0;
}