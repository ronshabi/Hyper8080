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

void
xthl(struct cpu *c)
{
	uint8_t l = c->l;
	uint8_t h = c->h;

	c->l = cpu_deref_sp(c, 0);
	c->h = cpu_deref_sp(c, 1);

	cpu_set_byte(c, c->sp, l);
	cpu_set_byte(c, c->sp + 1, h);
}
void
sphl(struct cpu *c)
{
	c->sp = cpu_get_hl(c);
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
inr_m(struct cpu *c)
{
	uint16_t result = cpu_deref_hl(c) + 1;
	cpu_set_byte(c, cpu_get_hl(c), result & 0xff);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
}
void
dcr(struct cpu *c, uint8_t *reg)
{
	uint16_t result = *reg + FLIP(1);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result & 0xff);
	*reg = result & 0xff;
}
void
dcr_m(struct cpu *c)
{
	uint16_t result = cpu_deref_hl(c) + FLIP(1);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result & 0xff);
	cpu_set_byte(c, cpu_get_hl(c), result & 0xff);
}
void
daa(struct cpu *c)
{
	uint16_t result = 0;
	if ((c->a & 0x0f) > 9 || c->flag_ac) {
		result += 6;
	}

	if (((c->a >> 4) > 9) || (c->flag_c)) {
		result += 0x60;
		c->flag_c = 1;
		result += c->a;
		cpu_flags_set_zsp(c, result & 0xff);
		cpu_flags_set_carry_from_word(c, result);
	}
}

/* ROTATE ACCUMULATOR */
void
rlc(struct cpu *c)
{
	uint8_t bit7 = (c->a & 0x80) >> 7;
	c->a <<= 1;
	c->a |= bit7;
	c->flag_c = bit7;
}
void
rrc(struct cpu *c)
{
	uint8_t bit0 = (c->a & 0x1);
	c->a >>= 1;
	c->a |= bit0 << 7;
	c->flag_c = (bit0 != 0);
}
void
ral(struct cpu *c)
{
	uint8_t bit7 = (c->a & 0x80) >> 7;
	c->a <<= 1;
	c->a |= c->flag_c;
	c->flag_c = bit7;
}
void
rar(struct cpu *c)
{
	uint8_t bit0 = c->a & 0x1;
	c->a = c->a >> 1;
	c->a = (c->a | (c->flag_c << 7));
	c->flag_c = bit0;
}

/* I/O */


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
add_m(struct cpu *c)
{
	uint16_t result = c->a + cpu_deref_hl(c);
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
adc_m(struct cpu *c)
{
	uint16_t result = c->a + cpu_deref_hl(c) + c->flag_c;
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
