#include "cpu.h"
#include <stdlib.h>

const uint8_t CYCLES[] = {
	4,	10, 7,	5,	5,	5,	7,	4,	4,	10, 7,	5,	5,	5,	7,	4,	4,	10, 7,
	5,	5,	5,	7,	4,	4,	10, 7,	5,	5,	5,	7,	4,	4,	10, 16, 5,	5,	5,
	7,	4,	4,	10, 16, 5,	5,	5,	7,	4,	4,	10, 13, 5,	10, 10, 10, 4,	4,
	10, 13, 5,	5,	5,	7,	4,	5,	5,	5,	5,	5,	5,	7,	5,	5,	5,	5,	5,
	5,	5,	7,	5,	5,	5,	5,	5,	5,	5,	7,	5,	5,	5,	5,	5,	5,	5,	7,
	5,	5,	5,	5,	5,	5,	5,	7,	5,	5,	5,	5,	5,	5,	5,	7,	5,	7,	7,
	7,	7,	7,	7,	7,	7,	5,	5,	5,	5,	5,	5,	7,	5,	4,	4,	4,	4,	4,
	4,	7,	4,	4,	4,	4,	4,	4,	4,	7,	4,	4,	4,	4,	4,	4,	4,	7,	4,
	4,	4,	4,	4,	4,	4,	7,	4,	4,	4,	4,	4,	4,	4,	7,	4,	4,	4,	4,
	4,	4,	4,	7,	4,	4,	4,	4,	4,	4,	4,	7,	4,	4,	4,	4,	4,	4,	4,
	7,	4,	5,	10, 10, 10, 11, 11, 7,	11, 5,	10, 10, 10, 11, 11, 7,	11, 5,
	10, 10, 10, 11, 11, 7,	11, 5,	10, 10, 10, 11, 11, 7,	11, 5,	10, 10, 18,
	11, 11, 7,	11, 5,	5,	10, 5,	11, 11, 7,	11, 5,	10, 10, 4,	11, 11, 7,
	11, 5,	5,	10, 4,	11, 11, 7,	11,
};

void cpu_init(cpu *c)
{
	c->a = 0;
	c->b = 0;
	c->c = 0;
	c->d = 0;
	c->e = 0;
	c->h = 0;
	c->l = 0;

	c->flag_z = 0;
	c->flag_s = 0;
	c->flag_p = 0;
	c->flag_c = 0;
	c->flag_ac = 0;

	c->pc = 0;
	c->sp = 0;
	c->memory = NULL;

	c->cycle_count = 0;
	c->halt = 0;
	c->interrupts_enabled = 0;

	c->i0 = 0;
	c->i1 = 0;
	c->i2 = 0;
	c->i3 = 0;
	c->o2 = 0;
	c->o3 = 0;
	c->o4 = 0;
	c->o5 = 0;
	c->o6 = 0;

	c->shift = 0;
	c->shift_amt = 0;
}

void cpu_set_memory(cpu *c, uint8_t *memory_ptr)
{
	c->memory = memory_ptr;
}

uint8_t cpu_get_byte(cpu *c, uint16_t address)
{
	return c->memory[address];
}

void cpu_set_byte(cpu *c, uint16_t address, uint8_t val)
{
	c->memory[address] = val;
}

uint16_t cpu_get_word(cpu *c, uint16_t address)
{
	return c->memory[address + 1] << 8 | c->memory[address];
}

void cpu_set_word(cpu *c, uint16_t address, uint16_t val)
{
	cpu_set_byte(c, address, val & 0xff);
	cpu_set_byte(c, address + 1, val >> 8);
}

uint16_t cpu_get_bc(cpu *c)
{
	return c->b << 8 | c->c;
}

uint16_t cpu_get_de(cpu *c)
{
	return c->d << 8 | c->e;
}

uint16_t cpu_get_hl(cpu *c)
{
	return c->h << 8 | c->l;
}

void cpu_set_bc(cpu *c, uint16_t val)
{
	c->b = val >> 8;
	c->c = val & 0xff;
}

void cpu_set_de(cpu *c, uint16_t val)
{
	c->d = val >> 8;
	c->e = val & 0xff;
}

void cpu_set_hl(cpu *c, uint16_t val)
{
	c->h = val >> 8;
	c->l = val & 0xff;
}

uint16_t cpu_deref_bc(cpu *c)
{
	return cpu_get_byte(c, cpu_get_bc(c));
}

uint16_t cpu_deref_de(cpu *c)
{
	return cpu_get_byte(c, cpu_get_de(c));
}

uint16_t cpu_deref_hl(cpu *c)
{
	return cpu_get_byte(c, cpu_get_hl(c));
}

void cpu_stack_push(cpu *c, uint16_t val)
{
	c->sp -= 2;
	cpu_set_word(c, c->sp, val);
}

uint16_t cpu_stack_pop(cpu *c)
{
	uint16_t ret = cpu_get_word(c, c->sp);
	c->sp += 2;
	return ret;
}
