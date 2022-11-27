#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	cpu_init (&c);
	c.a		 = 0x3d;
	c.c		 = 0x42;
	c.flag_c = 0;

	adc (&c, &(c.c));

	int passed = 0;

	if (c.a == 0x7f && !c.flag_c && !c.flag_s && !c.flag_z && !c.flag_p && !c.flag_ac) { passed++; }

	cpu_init (&c);
	c.a		 = 0x3d;
	c.c		 = 0x42;
	c.flag_c = 1;
	adc (&c, &(c.c));

	if (c.a == 0x80 && !c.flag_c && c.flag_s && !c.flag_z && !c.flag_p) { passed++; }

	if (passed == 2) { return 0; }

	return 1;
}