#include "defines.h"

void
inst_jmp(struct cpu *c, bool condition, uint16_t addr)
{
	debug_address(addr);
	c->pc += 2;
	if (condition) {
		c->pc = addr;
	}
}

void
inst_call(struct cpu *c, bool condition, uint16_t addr)
{
	debug_address(addr);
	c->pc += 2;
	if (condition) {
		cpu_stack_push(c, c->pc);
		c->pc = addr;
	}
}

void
inst_ret(struct cpu *c, bool condition)
{
	if (condition)
		c->pc = cpu_stack_pop(c);
}

/* SINGLE REGISTER INSTRUCTIONS */
void
inr(struct cpu *c, uint8_t *reg)
{
	uint16_t result = ++*reg;
	cpu_flags_set_zsp(c, *reg);
	cpu_flags_set_carry_from_word(c, result);
	*reg = result & 0xff;
}

void
dcr(struct cpu *c, uint8_t *reg)
{
	uint16_t result = *reg + FLIP(1);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result & 0xff);
	*reg = result & 0xff;
}

/* REGISTER OR MEMORY TO ACCUMULATOR */
void
add(struct cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + *reg;
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}

void
adc(struct cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + *reg + c->flag_c;
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}

void
sub(struct cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP(*reg);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
sub_m(struct cpu *c)
{
	uint16_t result = c->a + FLIP(cpu_deref_hl(c));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
sbb(struct cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP(*reg) + FLIP(c->flag_c);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
sbb_m(struct cpu *c)
{
	uint16_t result = c->a + FLIP(cpu_deref_hl(c)) + FLIP(c->flag_c);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
ana(struct cpu *c, const uint8_t *reg)
{
	uint16_t result = (c->a & (*reg));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
ana_m(struct cpu *c)
{
	uint16_t result = c->a & (cpu_deref_hl(c));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
xra(struct cpu *c, const uint8_t *reg)
{
	uint16_t result = (c->a ^ (*reg));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}

void
ora(struct cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a | *reg;
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}

void
cmp(struct cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP(*reg);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
}
