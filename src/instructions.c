#include "defs.h"

void
jmp(cpu *c, bool condition, uint16_t addr)
{
	debug_address(addr);
	PC2;
	if (condition) {
		c->pc = addr;
	}
}

void
call(cpu *c, bool condition, uint16_t addr)
{
	debug_address(addr);
	PC2;
	if (condition) {
		cpu_stack_push(c, c->pc);
		c->pc = addr;
	}
}

void
ret(cpu *c, bool condition)
{
	if (condition)
		c->pc = cpu_stack_pop(c);
}

void
adi(cpu *c)
{
	uint16_t result = c->a + ARG8;
	c->a = result & 0xff;
	cpu_flags_set_carry_from_word(c, result);
	cpu_flags_set_zsp(c, result);
	PC1;
}
void
aci(cpu *c)
{
	uint16_t result = c->a + ARG8 + c->flag_c;
	c->a = result & 0xff;
	cpu_flags_set_carry_from_word(c, result);
	cpu_flags_set_zsp(c, c->a);
	PC1;
}
void
sui(cpu *c)
{
	uint16_t result = c->a + FLIP(ARG8);
	c->a = result & 0xff;
	cpu_flags_set_carry_from_word(c, result);
	cpu_flags_set_zsp(c, c->a);
	PC1;
}
void
sbi(cpu *c)
{
	uint16_t result = c->a + FLIP(ARG8) + FLIP(c->flag_c);
	c->a = result & 0xff;
	cpu_flags_set_carry_from_word(c, result);
	cpu_flags_set_zsp(c, c->a);
	PC1;
}
void
ani(cpu *c)
{
	uint16_t result = c->a & ARG8;
	c->a = result & 0xff;
	cpu_flags_set_carry_from_word(c, result);
	cpu_flags_set_zsp(c, c->a);
	PC1;
}
void
xri(cpu *c)
{
	uint16_t result = c->a ^ ARG8;
	c->a = result & 0xff;
	cpu_flags_set_carry_from_word(c, result);
	cpu_flags_set_zsp(c, c->a);
	PC1;
}
void
ori(cpu *c)
{
	uint16_t result = c->a | ARG8;
	c->a = result & 0xff;
	cpu_flags_set_carry_from_word(c, result);
	cpu_flags_set_zsp(c, result & 0xff);
	PC1;
}
void
cpi(cpu *c)
{
	/* A is not changed by this operation, only the FLAGS */
	uint16_t result = c->a + FLIP(ARG8);
	cpu_flags_set_carry_from_word(c, result);
	cpu_flags_set_zsp(c, result & 0xff);
	PC1;
}

/* DATA TRANSFER */
void
dad_b(cpu *c)
{
	uint16_t result = cpu_get_bc(c) + cpu_get_hl(c);
	cpu_flags_set_carry_from_word(c, result);
	cpu_set_hl(c, result);
}
void
dad_d(cpu *c)
{
	uint32_t result = cpu_get_de(c) + cpu_get_hl(c);
	cpu_flags_set_carry_from_word(c, result & 0xffff);
	cpu_set_hl(c, result & 0xffff);
}
void
dad_h(cpu *c)
{
	uint32_t result = cpu_get_hl(c) + cpu_get_hl(c);
	cpu_flags_set_carry_from_word(c, result & 0xffff);
	cpu_set_hl(c, result & 0xffff);
}
void
dad_sp(cpu *c)
{
	uint32_t result = c->sp + cpu_get_hl(c);
	cpu_flags_set_carry_from_word(c, result & 0xffff);
	cpu_set_hl(c, result & 0xffff);
}
void
inx_b(cpu *c)
{
	cpu_set_bc(c, cpu_get_bc(c) + 1);
}
void
inx_d(cpu *c)
{
	cpu_set_de(c, cpu_get_de(c) + 1);
}
void
inx_h(cpu *c)
{
	cpu_set_hl(c, cpu_get_hl(c) + 1);
}
void
inx_sp(cpu *c)
{
	c->sp++;
}
void
dcx_b(cpu *c)
{
	cpu_set_bc(c, cpu_get_bc(c) - 1);
}
void
dcx_d(cpu *c)
{
	cpu_set_de(c, cpu_get_de(c) - 1);
}
void
dcx_h(cpu *c)
{
	cpu_set_hl(c, cpu_get_hl(c) - 1);
}
void
dcx_sp(cpu *c)
{
	c->sp--;
}
void
xchg(cpu *c)
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
xthl(cpu *c)
{
	uint8_t l = c->l;
	uint8_t h = c->h;

	c->l = cpu_deref_sp(c, 0);
	c->h = cpu_deref_sp(c, 1);

	cpu_set_byte(c, c->sp, l);
	cpu_set_byte(c, c->sp + 1, h);
}
void
sphl(cpu *c)
{
	c->sp = cpu_get_hl(c);
}

/* SINGLE REGISTER INSTRUCTIONS */
void
inr(cpu *c, uint8_t *reg)
{
	uint16_t result = ++*reg;
	cpu_flags_set_zsp(c, *reg);
	cpu_flags_set_carry_from_word(c, result);
	*reg = result & 0xff;
}
void
inr_m(cpu *c)
{
	uint16_t result = cpu_deref_hl(c) + 1;
	cpu_set_byte(c, cpu_get_hl(c), result & 0xff);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
}
void
dcr(cpu *c, uint8_t *reg)
{
	uint16_t result = *reg + FLIP(1);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result & 0xff);
	*reg = result & 0xff;
}
void
dcr_m(cpu *c)
{
	uint16_t result = cpu_deref_hl(c) + FLIP(1);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result & 0xff);
	cpu_set_byte(c, cpu_get_hl(c), result & 0xff);
}
void
daa(cpu *c)
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
rlc(cpu *c)
{
	uint8_t bit7 = (c->a & 0x80) >> 7;
	c->a <<= 1;
	c->a |= bit7;
	c->flag_c = bit7;
}
void
rrc(cpu *c)
{
	uint8_t bit0 = (c->a & 0x1);
	c->a >>= 1;
	c->a |= bit0 << 7;
	c->flag_c = (bit0 != 0);
}
void
ral(cpu *c)
{
	uint8_t bit7 = (c->a & 0x80) >> 7;
	c->a <<= 1;
	c->a |= c->flag_c;
	c->flag_c = bit7;
}
void
rar(cpu *c)
{
	uint8_t bit0 = c->a & 0x1;
	c->a = c->a >> 1;
	c->a = (c->a | (c->flag_c << 7));
	c->flag_c = bit0;
}

/* I/O */
void
in(cpu *c)
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
out(cpu *c)
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
hlt(cpu *c)
{
	debug_msg("CPU Halted");
	c->halt = 1;
}

/* REGISTER OR MEMORY TO ACCUMULATOR */
void
add(cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + *reg;
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
add_m(cpu *c)
{
	uint16_t result = c->a + cpu_deref_hl(c);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
adc(cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + *reg + c->flag_c;
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
adc_m(cpu *c)
{
	uint16_t result = c->a + cpu_deref_hl(c) + c->flag_c;
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
sub(cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP(*reg);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
sub_m(cpu *c)
{
	uint16_t result = c->a + FLIP(cpu_deref_hl(c));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
sbb(cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP(*reg) + FLIP(c->flag_c);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
sbb_m(cpu *c)
{
	uint16_t result = c->a + FLIP(cpu_deref_hl(c)) + FLIP(c->flag_c);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
ana(cpu *c, const uint8_t *reg)
{
	uint16_t result = (c->a & (*reg));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
ana_m(cpu *c)
{
	uint16_t result = c->a & (cpu_deref_hl(c));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
xra(cpu *c, const uint8_t *reg)
{
	uint16_t result = (c->a ^ (*reg));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
xra_m(cpu *c)
{
	uint16_t result = c->a ^ (cpu_deref_hl(c));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
ora(cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a | *reg;
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
ora_m(cpu *c)
{
	uint16_t result = c->a | (cpu_deref_hl(c));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
	c->a = result & 0xff;
}
void
cmp(cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP(*reg);
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
}

void
cmp_m(cpu *c)
{
	uint16_t result = c->a + FLIP(cpu_deref_hl(c));
	cpu_flags_set_zsp(c, result & 0xff);
	cpu_flags_set_carry_from_word(c, result);
}
