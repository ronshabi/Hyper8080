#include <stdlib.h>
#include "../src/instructions.h"
#include "../src/instructions.c"
#include "../src/cpu.h"
#include "../src/cpu.c"

int main (void)
{
	cpu c;
	C_Init (&c);

	uint8_t *memory = calloc (10000, 1);
	C_SetMemory (&c, memory);

	int tests_passed = 0;

	// DAD B

	c.b = 0x33;
	c.c = 0x9f;
	c.h = 0xa1;
	c.l = 0x7b;
	dad_b (&c);
	if (C_GetHL (&c) == 0xD51A && c.flag_c == 0)
	{
		printf ("DAD Passed!\n");
		tests_passed++;
	}

	// INX D
	c.d = 0x38;
	c.e = 0xff;

	inx_d (&c);

	if (C_GetDE (&c) == 0x3900)
	{
		printf ("INX Passed!\n");
		tests_passed++;
	}

	// DCX H
	c.h = 0x98;
	c.l = 0x00;

	dcx_h (&c);

	if (C_GetHL (&c) == 0x97ff)
	{
		printf ("DCX Passed!\n");
		tests_passed++;
	}

	// INX SP
	c.sp = 0xffff;
	inx_sp (&c);
	if (c.sp == 0)
	{
		printf ("INX SP Passed!\n");
		tests_passed++;
	}

	// XCHG

	c.h = 0;
	c.l = 0xff;
	c.d = 0x33;
	c.e = 0x55;

	xchg (&c);

	if (c.d == 00 && c.e == 0xff && c.h == 0x33 && c.l == 0x55)
	{
		printf ("XCHG Passed!\n");
		tests_passed++;
	}

	// XTHL
	c.sp = 0x10ad;
	c.h	 = 0x0b;
	c.l	 = 0x3c;
	C_SetByte (&c, 0x10ad, 0xf0);
	C_SetByte (&c, 0x10ae, 0x0d);

	xthl (&c);

	if (c.h == 0x0d && c.l == 0xf0 && C_DerefSP (&c, 0) == 0x3c && C_DerefSP (&c, 1) == 0x0b)
	{
		printf ("XTHL Passed!\n");
		tests_passed++;
	}

	// SPHL

	c.h = 0x50;
	c.l = 0x6c;

	sphl (&c);
	if (c.sp == 0x506c)
	{
		printf ("SPHL Passed!\n");
		tests_passed++;
	}

	if (tests_passed == 7) { return 0; }

	return 1;
}