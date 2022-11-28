#include "defines.h"

void
debug_address(uint16_t addr)
{
#ifdef DEBUG_MODE_REGULAR
	printf(" $%04x", addr);
#endif
}

void
debug_device(uint8_t dev)
{
#ifdef DEBUG_MODE_REGULAR
	printf(" <Device: %d>", dev);
#endif
}

void
debug_msg(const char *msg)
{
#ifdef DEBUG_MODE_REGULAR
	printf("\n%s\n", msg);
#endif
}

void
debug_disassemble(struct cpu *c)
{
#ifdef DEBUG_MODE_REGULAR
	printf(
	    "0x%04x\t%02x\t\t				\
	    %02x|%02x|%02x|%02x|%02x|%02x|%02x|%04x|	\
	    %d%d%d%d%d					\
	    |%04x					\
	    \t%s",					\

	    c->pc, cpu_get_byte(c, c->pc),
		c->a, c->b, c->c, c->d, c->e, c->h, c->l, c->sp,
		c->flag_z, c->flag_s, c->flag_p, c->flag_c, c->flag_ac,
	    	c->shift,
		C_INSTRUCTIONS[cpu_get_byte(c, c->pc)]);
#endif
}

void
debug_stop(struct cpu *c, bool *quit)
{
#ifdef DEBUG_MODE_STOP
	if (c->instructions == DEBUG_MODE_STOP_AT_INSTRUCTION) {
		*quit = true;
	}
#endif
}

void
debug_summary(struct cpu *c)
{
#ifdef DEBUG_MODE_STOP
	printf("Stopped at instruction %lu\n", c->instructions);
	printf("Cycles: %lu\n", c->cycles);
#endif
}
