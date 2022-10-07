#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.h"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	cpu_init (&c);

	uint8_t *memory = calloc (10000, 1);
	C_SetMemory (&c, memory);

	int passed = 0;
	
	c.flag_c = 1;

	cmc (&c);
	
	if (c.flag_c == 0) {
		passed++;
	}

	cmc (&c);
	
	if (c.flag_c == 1) {
		passed++;
	}
	
	if (passed == 2) {
		return 0;
	}
	
	free(memory);
	return 1;
}