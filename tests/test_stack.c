#include "../src/cpu.c"
#include "../src/cpu.h"
#include <stdlib.h>

int main(void)
{
	cpu c;
	cpu_init(&c);

	uint8_t *memory = malloc(10000);
	cpu_set_memory(&c, memory);
	cpu_stack_push(&c, 0xcafe);

	if (cpu_stack_pop(&c) == 0xcafe)
	{
		return 0;
	}

	return 1;
}