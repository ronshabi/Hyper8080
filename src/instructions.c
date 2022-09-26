#include "instructions.h"

/* JUMP */
void pchl (cpu *c) { cpu_unimplemented (c); }
void jmp (cpu *c) { c->pc = cpu_get_word (c, c->pc + 1); }
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
	stack_push (c, c->pc);
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
	PC1;
}
void mvi_m (cpu *c)
{
	cpu_set_byte (c, cpu_deref_hl (c), cpu_get_byte (c, c->pc + 1));
	PC1;
}
