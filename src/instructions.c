#include "defines.h"

void
inst_jmp(struct cpu *c, bool condition, uint16_t addr)
{
	debug_address(addr);
	PC2;
	if (condition) {
		c->pc = addr;
	}
}

void
inst_call(struct cpu *c, bool condition, uint16_t addr)
{
	debug_address(addr);
	PC2;
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
dad_sp(struct cpu *c)
{
	uint32_t result = c->sp + cpu_get_hl(c);
	cpu_flags_set_carry_from_word(c, result & 0xffff);
	cpu_set_hl(c, result & 0xffff);
}
void
inx_b(struct cpu *c)
{
	cpu_set_bc(c, cpu_get_bc(c) + 1);
}
void
inx_d(struct cpu *c)
{
	cpu_set_de(c, cpu_get_de(c) + 1);
}
void
inx_h(struct cpu *c)
{
	cpu_set_hl(c, cpu_get_hl(c) + 1);
}
void
inx_sp(struct cpu *c)
{
	c->sp++;
}
void
dcx_b(struct cpu *c)
{
	cpu_set_bc(c, cpu_get_bc(c) - 1);
}
void
dcx_d(struct cpu *c)
{
	cpu_set_de(c, cpu_get_de(c) - 1);
}
void
dcx_h(struct cpu *c)
{
	cpu_set_hl(c, cpu_get_hl(c) - 1);
}
void
dcx_sp(struct cpu *c)
{
	c->sp--;
}
void
xchg(struct cpu *c)
{
	uint8_t temp;
	temp = c->h;
	c->h = c->d;
	c->d = temp;

	temp = c->l;
	c->l = c->e;
	c->e = temp;
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
void
in(struct cpu *c)
{
	uint8_t device_number = ARG8;
	PC1;

	debug_device(device_number);

	if (device_number == DEVICE_INP0) {
		c->a = c->i0;
	} else if (device_number == DEVICE_INP1) {
		c->a = c->i1;
	} else if (device_number == DEVICE_INP2) {
		c->a = c->i2;
	} else if (device_number == DEVICE_SHIFT_IN) {
		c->a = (c->shift >> ((8 - c->shift_amt)) & 0xff);
	}
}
void
out(struct cpu *c)
{
	uint8_t device_number = ARG8;
	PC1;

	debug_device(device_number);

	if (device_number == DEVICE_SHIFT_AMT) {
		c->shift_amt = (c->a & 7);
	} else if (device_number == DEVICE_SOUND1) {
		c->o3 = c->a;
	} else if (device_number == DEVICE_SHIFT_DATA) {
		c->shift >>= 8;
		c->shift |= (c->a << 8);
	} else if (device_number == DEVICE_SOUND2) {
		c->o5 = c->a;
	} else if (device_number == DEVICE_WATCHDOG) {
		c->o6 = c->a;
	}
}

void
hlt(struct cpu *c)
{
	debug_msg("CPU Halted");
	c->halt = 1;
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
xra_m(struct cpu *c)
{
	uint16_t result = c->a ^ (cpu_deref_hl(c));
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
ora_m(struct cpu *c)
{
	uint16_t result = c->a | (cpu_deref_hl(c));
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

void
cmp_m(struct cpu *c)
{
	uint16_t result = c->a + FLIP(cpu_deref_hl(c));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
}
