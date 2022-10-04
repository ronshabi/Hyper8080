#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>

/* CARRY */
void stc (cpu *c)
{
	c->flag_c = 1;
	PC1;
}

void cmc (cpu *c)
{
	if (c->flag_c == 1) { c->flag_c = 0; }
	else { c->flag_c = 1; }
	PC1;
}

/* JUMP */
void pchl (cpu *c) { cpu_unimplemented (c); }
void jmp (cpu *c)
{
	printf (" $%04x", cpu_get_word (c, c->pc + 1));
	c->pc = cpu_get_word (c, c->pc + 1);
}
void jc (cpu *c)
{
	if (c->flag_c) { jmp (c); }
	else { PC3; }
}
void jnc (cpu *c)
{
	if (!c->flag_c) { jmp (c); }
	else { PC3; }
}
void jz (cpu *c)
{
	if (c->flag_z) { jmp (c); }
	else { PC3; }
}
void jnz (cpu *c)
{
	if (!c->flag_z) { jmp (c); }
	else { PC3; }
}
void jm (cpu *c)
{
	if (c->flag_s) { jmp (c); }
	else { PC3; }
}
void jp (cpu *c)
{
	if (!c->flag_s) { jmp (c); }
	else { PC3; }
}
void jpe (cpu *c)
{
	if (c->flag_p) { jmp (c); }
	else { PC3; }
}
void jpo (cpu *c)
{
	if (!c->flag_p) { jmp (c); }
	else { PC3; }
}

/* CALL */
void call (cpu *c)
{
	// +3
	stack_push (c, c->pc + 3);
	jmp (c);
}
void cc (cpu *c)
{
	if (c->flag_c) { call (c); }
	else { PC3; }
}
void cnc (cpu *c)
{
	if (!c->flag_c) { call (c); }
	else { PC3; }
}
void cz (cpu *c)
{
	if (c->flag_z) { call (c); }
	else { PC3; }
}
void cnz (cpu *c)
{
	if (!c->flag_z) { call (c); }
	else { PC3; }
}
void cm (cpu *c)
{
	if (!c->flag_s) { call (c); }
	else { PC3; }
}
void cp (cpu *c)
{
	if (!c->flag_s) { call (c); }
	else { PC3; }
}
void cpe (cpu *c)
{
	if (c->flag_p) { call (c); }
	else { PC3; }
}
void cpo (cpu *c)
{
	if (!c->flag_p) { call (c); }
	else { PC3; }
}

/* RET */
void ret (cpu *c) { c->pc = stack_pop (c); }
void rc (cpu *c)
{
	if (c->flag_c) { ret (c); }
	else { PC1; }
}
void rnc (cpu *c)
{
	if (!c->flag_c) { ret (c); }
	else { PC1; }
}
void rz (cpu *c)
{
	if (c->flag_z) { ret (c); }
	else { PC1; }
}
void rnz (cpu *c)
{
	if (!c->flag_z) { ret (c); }
	else { PC1; }
}
void rm (cpu *c)
{
	if (c->flag_s) { ret (c); }
	else { PC1; }
}
void rp (cpu *c)
{
	if (!c->flag_s) { ret (c); }
	else { PC1; }
}
void rpe (cpu *c)
{
	if (c->flag_p) { ret (c); }
	else { PC1; }
}
void rpo (cpu *c)
{
	if (!c->flag_p) { ret (c); }
	else { PC1; }
}

/* IMMEDIATE INSTRUCTIONS */
void lxi_b (cpu *c)
{
	cpu_set_bc (c, cpu_get_word (c, c->pc + 1));
	PC3;
}
void lxi_d (cpu *c)
{
	cpu_set_de (c, cpu_get_word (c, c->pc + 1));
	PC3;
}
void lxi_h (cpu *c)
{
	cpu_set_hl (c, cpu_get_word (c, c->pc + 1));
	PC3;
}
void lxi_sp (cpu *c)
{
	c->sp = cpu_get_word (c, c->pc + 1);
	PC3;
}
void mvi (cpu *c, uint8_t *reg)
{
	*reg = cpu_get_byte (c, c->pc + 1);
	PC2;
}
void mvi_m (cpu *c)
{
	cpu_set_byte (c, cpu_deref_hl (c), cpu_get_byte (c, c->pc + 1));
	PC2;
}
void adi (cpu *c)
{
	uint16_t result = c->a + GET_IMMEDIATE_BYTE;
	c->a			= result & 0xff;
	cpu_set_flags_carry_from_16bit (c, result);
	cpu_set_flags_zsp (c, result);
	PC2;
}
void aci (cpu *c)
{
	uint16_t result = c->a + GET_IMMEDIATE_BYTE + c->flag_c;
	c->a			= result & 0xff;
	cpu_set_flags_carry_from_16bit (c, result);
	cpu_set_flags_zsp (c, c->a);
	PC2;
}
void sui (cpu *c)
{
	uint16_t result = c->a + (~GET_IMMEDIATE_BYTE + 1);
	c->a			= result & 0xff;
	cpu_set_flags_carry_from_16bit (c, result);
	cpu_set_flags_zsp (c, c->a);
	PC2;
}
void sbi (cpu *c)
{
	uint16_t result = c->a + (~GET_IMMEDIATE_BYTE + 1) + (~c->flag_c + 1);
	c->a			= result & 0xff;
	cpu_set_flags_carry_from_16bit (c, result);
	cpu_set_flags_zsp (c, c->a);
	PC2;
}
void ani (cpu *c)
{
	uint16_t result = c->a & GET_IMMEDIATE_BYTE;
	c->a			= result & 0xff;
	cpu_set_flags_carry_from_16bit (c, result);
	cpu_set_flags_zsp (c, c->a);
	PC2;
}
void xri (cpu *c)
{
	uint16_t result = c->a ^ GET_IMMEDIATE_BYTE;
	c->a			= result & 0xff;
	cpu_set_flags_carry_from_16bit (c, result);
	cpu_set_flags_zsp (c, c->a);
	PC2;
}
void ori (cpu *c)
{
	uint16_t result = c->a | GET_IMMEDIATE_BYTE;
	c->a			= result & 0xff;
	cpu_set_flags_carry_from_16bit (c, result);
	cpu_set_flags_zsp (c, c->a);
	PC2;
}
void cpi (cpu *c)
{
	// A is not changed by this operation, only the FLAGS
	uint16_t result = c->a + (~GET_IMMEDIATE_BYTE + 1);
	cpu_set_flags_carry_from_16bit (c, result);
	cpu_set_flags_zsp (c, result & 0xff);
	PC2;
}

/* DATA TRANSFER */
void ldax_b (cpu *c)
{
	c->a = cpu_deref_bc (c);
	PC1;
}
void ldax_d (cpu *c)
{
	c->a = cpu_deref_de (c);
	PC1;
}
void mov (cpu *c, uint8_t *dest, const uint8_t *src)
{
	*dest = *src;
	PC1;
}
void mov_m_to_dest (cpu *c, uint8_t *dest)
{
	uint8_t m = cpu_deref_hl (c);
	*dest	  = m;
	PC1;
}
void mov_m (cpu *c, const uint8_t *reg)
{
	cpu_set_byte (c, cpu_deref_hl (c), *reg);
	PC1;
}
void stax_b (cpu *c)
{
	cpu_set_byte (c, cpu_get_bc (c), c->a);
	PC1;
}
void stax_d (cpu *c)
{
	cpu_set_byte (c, cpu_get_de (c), c->a);
	PC1;
}
/* REGISTER PAIR INSTRUCTIONS */
void push_b (cpu *c)
{
	stack_push (c, cpu_get_bc (c));
	PC1;
}
void push_d (cpu *c)
{
	stack_push (c, cpu_get_de (c));
	PC1;
}
void push_h (cpu *c)
{
	stack_push (c, cpu_get_hl (c));
	PC1;
}
void push_psw (cpu *c)
{
	stack_push_psw (c);
	PC1;
}
void pop_b (cpu *c)
{
	cpu_set_bc (c, stack_pop (c));
	PC1;
}

void pop_d (cpu *c)
{
	cpu_set_de (c, stack_pop (c));
	PC1;
}
void pop_h (cpu *c)
{
	cpu_set_hl (c, stack_pop (c));
	PC1;
}
void pop_psw (cpu *c)
{
	stack_pop_psw (c);
	PC1;
}
void dad_b (cpu *c)
{
	uint32_t result = cpu_get_bc (c) + cpu_get_hl (c);
	if (result > 0xffff) { c->flag_c = 1; }
	else { c->flag_c = 0; }
	cpu_set_hl (c, result & 0xffff);
	PC1;
}
void dad_d (cpu *c)
{
	uint32_t result = cpu_get_de (c) + cpu_get_hl (c);
	if (result > 0xffff) { c->flag_c = 1; }
	else { c->flag_c = 0; }
	cpu_set_hl (c, result & 0xffff);
	PC1;
}
void dad_h (cpu *c)
{
	uint32_t result = cpu_get_hl (c) + cpu_get_hl (c);
	if (result > 0xffff) { c->flag_c = 1; }
	else { c->flag_c = 0; }
	cpu_set_hl (c, result & 0xffff);
	PC1;
}
void dad_sp (cpu *c)
{
	uint32_t result = c->sp + cpu_get_hl (c);
	if (result > 0xffff) { c->flag_c = 1; }
	else { c->flag_c = 0; }
	cpu_set_hl (c, result & 0xffff);
	PC1;
}
void inx_b (cpu *c)
{
	cpu_set_bc (c, cpu_get_bc (c) + 1);
	PC1;
}
void inx_d (cpu *c)
{
	cpu_set_de (c, cpu_get_de (c) + 1);
	PC1;
}
void inx_h (cpu *c)
{
	cpu_set_hl (c, cpu_get_hl (c) + 1);
	PC1;
}
void inx_sp (cpu *c)
{
	c->sp++;
	PC1;
}
void dcx_b (cpu *c)
{
	cpu_set_bc (c, cpu_get_bc (c) - 1);
	PC1;
}
void dcx_d (cpu *c)
{
	cpu_set_de (c, cpu_get_de (c) - 1);
	PC1;
}
void dcx_h (cpu *c)
{
	cpu_set_hl (c, cpu_get_hl (c) - 1);
	PC1;
}
void dcx_sp (cpu *c) { c->sp--; }
void xchg (cpu *c)
{
	uint8_t temp;
	temp = c->h;
	c->h = c->d;
	c->d = temp;

	temp = c->l;
	c->l = c->e;
	c->e = temp;

	PC1;
}
void xthl (cpu *c)
{
	uint8_t l = c->l;
	uint8_t h = c->h;

	c->l = cpu_deref_sp (c, 0);
	c->h = cpu_deref_sp (c, 1);

	cpu_set_byte (c, c->sp, l);
	cpu_set_byte (c, c->sp + 1, h);

	PC1;
}
void sphl (cpu *c)
{
	c->sp = cpu_get_hl (c);
	PC1;
}

/* SINGLE REGISTER INSTRUCTIONS */
void inr (cpu *c, uint8_t *reg)
{
	uint16_t result = ++*reg;
	cpu_set_flags_zsp (c, *reg);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void inr_m (cpu *c)
{
	uint16_t result = cpu_deref_hl (c) + 1;
	cpu_set_byte (c, cpu_deref_hl (c), result & 0xff);
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	PC1;
}
void dcr (cpu *c, uint8_t *reg)
{
	uint16_t result = *reg + FLIP (1);
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result & 0xff);
	*reg = result & 0xff;

	if (*reg == 0) { printf ("\nREG HIT ZERO\n"); }
	PC1;
}
void dcr_m (cpu *c)
{
	uint16_t result = cpu_deref_hl (c) + FLIP (1);
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result & 0xff);
	cpu_set_byte (c, cpu_get_hl (c), result & 0xff);

	if (cpu_deref_hl (c) == 0) { printf ("\nM HIT ZERO\n"); }
	PC1;
}

/* ROTATE ACCUMULATOR */
void rlc (cpu *c)
{
	uint8_t bit7 = (c->a & 0x80) >> 7;
	c->a <<= 1;
	c->a |= bit7;
	c->flag_c = bit7;
	PC1;
}
void rrc (cpu *c)
{
	uint8_t bit0 = (c->a & 0x1);
	c->a >>= 1;
	c->a |= bit0 << 7;
	c->flag_c = bit0;
	PC1;
}
void ral (cpu *c)
{
	uint8_t bit7 = (c->a & 0x80) >> 7;
	c->a <<= 1;
	c->a |= c->flag_c;
	c->flag_c = bit7;
	PC1;
}
void rar (cpu *c)
{
	uint8_t bit0 = (c->a & 0x1);
	uint8_t bit7 = (c->a & 0x80) >> 7;
	c->a >>= 1;
	c->a |= bit7 << 7;
	c->flag_c = bit0;
	PC1;
}

/* I/O */
void in (cpu *c)
{
	uint8_t device_number = cpu_get_byte (c, c->pc + 1);
	printf (" <DEVICE = %d>", device_number);

	if (device_number == DEVICE_INP0)
	{
		// IN DEVICE 0
		c->i0 = c->a;
	}
	else if (device_number == DEVICE_INP1)
	{
		// IN DEVICE 1
		c->i1 = c->a;
	}
	else if (device_number == DEVICE_INP2)
	{
		// IN DEVICE 2
		c->i2 = c->a;
	}
	else if (device_number == DEVICE_SHIFT_IN)
	{
		// IN DEVICE 3
		c->a = c->shift >> (8 - c->shift_amt);
	}

	PC2;
}
void out (cpu *c)
{
	uint8_t device_number = cpu_get_byte (c, c->pc + 1);

	printf (" <DEVICE = %d>", device_number);

	if (device_number == DEVICE_SHIFT_AMT)
	{
		// OUT DEVICE 2
		c->o2		 = c->a;
		c->shift_amt = c->a & 0b00000111; // bits 0-2
	}
	else if (device_number == DEVICE_SOUND1)
	{
		// OUT DEVICE 3
		c->o3 = c->a;
	}
	else if (device_number == DEVICE_SHIFT_DATA)
	{
		// OUT DEVICE 4
		c->o4 = c->a;
		c->shift >>= 8;
		c->shift |= c->a << 8;
	}
	else if (device_number == DEVICE_SOUND2)
	{
		// OUT DEVICE 5
		c->o5 = c->a;
	}
	else if (device_number == DEVICE_WATCHDOG)
	{
		// OUT DEVICE 6
		c->o6 = c->a;
	}
	PC2;
}
void hlt (cpu *c)
{
	printf ("\nHALTED!\n");
	c->halt = 1;
}

/* REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS */
void add (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + *reg;
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void add_m (cpu *c)
{
	uint16_t result = c->a + cpu_deref_hl (c);
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void adc (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + *reg + c->flag_c;
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void adc_m (cpu *c)
{
	uint16_t result = c->a + cpu_deref_hl (c) + c->flag_c;
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void sub (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP (*reg);
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void sub_m (cpu *c)
{
	uint16_t result = c->a + FLIP (cpu_deref_hl (c));
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void sbb (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP (*reg) + FLIP (c->flag_c);
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void sbb_m (cpu *c)
{
	uint16_t result = c->a + FLIP (cpu_deref_hl (c)) + FLIP (c->flag_c);
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void ana (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a & *reg;
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void ana_m (cpu *c)
{
	uint16_t result = c->a & (cpu_deref_hl (c));
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void xra (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a ^ *reg;
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void xra_m (cpu *c)
{
	uint16_t result = c->a ^ (cpu_deref_hl (c));
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void ora (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a | *reg;
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}
void ora_m (cpu *c)
{
	uint16_t result = c->a | (cpu_deref_hl (c));
	cpu_set_flags_zsp (c, result & 0xff);
	cpu_set_flags_carry_from_16bit (c, result);
	c->a = result & 0xff;
	PC1;
}

/* DIRECT ADDRESSING INSTRUCTIONS */
void sta (cpu *c)
{
	uint16_t adr = cpu_get_word (c, c->pc + 1);
	cpu_set_byte (c, adr, c->a);
	PC3;
}
void lda (cpu *c)
{
	uint16_t adr = cpu_get_word (c, c->pc + 1);
	printf (" $%04x", adr);
	c->a = cpu_get_byte (c, adr);

	PC3;
}
void shld (cpu *c)
{
	uint16_t adr = cpu_get_word (c, c->pc + 1);
	cpu_set_word (c, adr, cpu_get_hl (c));
	PC3;
}
void lhld (cpu *c)
{
	uint16_t adr  = cpu_get_word (c, c->pc + 1);
	uint16_t word = cpu_get_word (c, adr);
	cpu_set_hl (c, word);
	PC3;
}

/* INTERRUPT INSTRUCTIONS */
void set_interrupt (cpu *c, uint8_t state)
{
	c->interrupts_enabled = state;
	PC1;
}
