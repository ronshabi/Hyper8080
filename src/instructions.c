#include "instructions.h"
#include <stdio.h>
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
void ret (cpu *c)
{
	printf (" $%04x", cpu_get_word (c, c->pc + 1));
	c->pc = stack_pop (c);
}
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
	uint16_t temp = c->a + cpu_get_byte (c, c->pc + 1);
	c->a		  = temp & 0xff;
	// Set flags
	cpu_set_flags_zsp (c, c->a);
	cpu_set_flags_ac (c, c->a, cpu_get_byte (c, c->pc + 1), 0);
	cpu_set_flags_c (c, c->a, cpu_get_byte (c, c->pc + 1), 0);
	PC2;
}
void aci (cpu *c)
{
	uint16_t temp = (c->a) + (cpu_get_byte (c, c->pc + 1)) + c->flag_c;
	c->a		  = temp & 0xff;
	// Set flags
	cpu_set_flags_zsp (c, c->a);
	cpu_set_flags_ac (c, c->a, cpu_get_byte (c, c->pc + 1), c->flag_c);
	cpu_set_flags_c (c, c->a, cpu_get_byte (c, c->pc + 1), c->flag_c);
	PC2;
}
void sui (cpu *c)
{
	uint16_t temp = c->a - cpu_get_byte (c, c->pc + 1);
	c->a		  = temp & 0xff;
	// Set flags
	cpu_set_flags_zsp (c, c->a);
	cpu_set_flags_ac (c, c->a, -cpu_get_byte (c, c->pc + 1), 0);
	cpu_set_flags_c (c, c->a, -cpu_get_byte (c, c->pc + 1), 0);
	PC2;
}
void sbi (cpu *c)
{
	uint16_t temp = c->a - cpu_get_byte (c, c->pc + 1) - c->flag_c;
	c->a		  = temp & 0xff;
	// Set flags
	cpu_set_flags_zsp (c, c->a);
	cpu_set_flags_ac (c, c->a, -cpu_get_byte (c, c->pc + 1), -c->flag_c);
	cpu_set_flags_c (c, c->a, -cpu_get_byte (c, c->pc + 1), -c->flag_c);
	PC2;
}
void ani (cpu *c)
{
	uint16_t temp = c->a & cpu_get_byte (c, c->pc + 1);
	c->a		  = temp & 0xff;
	// Set flags
	cpu_set_flags_zsp (c, c->a);
	cpu_set_flags_c_and (c, c->a, cpu_get_byte (c, c->pc + 1));
	cpu_set_flags_ac_and (c, c->a, cpu_get_byte (c, c->pc + 1));
	PC2;
}
void xri (cpu *c)
{
	uint16_t temp = c->a ^ cpu_get_byte (c, c->pc + 1);
	c->a		  = temp & 0xff;
	// Set flags
	cpu_set_flags_zsp (c, c->a);
	cpu_set_flags_c_xor (c, c->a, cpu_get_byte (c, c->pc + 1));
	cpu_set_flags_ac_xor (c, c->a, cpu_get_byte (c, c->pc + 1));
	PC2;
}
void ori (cpu *c)
{
	uint16_t temp = c->a | cpu_get_byte (c, c->pc + 1);
	c->a		  = temp & 0xff;
	// Set flags
	cpu_set_flags_zsp (c, c->a);
	cpu_set_flags_c_or (c, c->a, cpu_get_byte (c, c->pc + 1));
	cpu_set_flags_ac_or (c, c->a, cpu_get_byte (c, c->pc + 1));
	PC2;
}
void cpi (cpu *c)
{
	// A is not changed by this operation
	cpu_set_flags_zsp (c, c->a);
	cpu_set_flags_ac (c, c->a, -cpu_get_byte (c, c->pc + 1), 0);
	cpu_set_flags_c (c, c->a, -cpu_get_byte (c, c->pc + 1), 0);
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
void mov_m (cpu *c, const uint8_t *reg)
{
	cpu_set_byte (c, cpu_deref_hl (c), *reg);
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
	*reg += 1;
	cpu_set_flags_zsp (c, *reg);
	cpu_set_flags_ac (c, *reg, 1, 0);
	PC1;
}
void inr_m (cpu *c)
{
	uint8_t m = cpu_deref_hl (c);
	cpu_set_byte (c, cpu_get_hl (c), m + 1);
	cpu_set_flags_zsp (c, m + 1);
	cpu_set_flags_ac (c, m, 1, 0);
	PC1;
}
void dcr (cpu *c, uint8_t *reg)
{
	uint16_t precision = *reg;
	precision--;
	*reg = precision & 0xff;
	cpu_set_flags_zsp (c, *reg);
	// cpu_set_flags_ac (c, *reg, -1, 0);

	if (*reg == 0) { printf ("\nREG HIT ZERO\n"); }

	PC1;
}
void dcr_m (cpu *c)
{
	uint8_t m = cpu_deref_hl (c);
	cpu_set_byte (c, cpu_get_hl (c), m - 1);
	cpu_set_flags_zsp (c, m - 1);
	cpu_set_flags_ac (c, m, -1, 0);
	PC1;
}
