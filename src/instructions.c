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
void dad_b (cpu *c) {}
void dad_d (cpu *c) {}
void dad_h (cpu *c) {}
void dad_sp (cpu *c) {}
void inx_b (cpu *c) {}
void inx_d (cpu *c) {}
void inx_h (cpu *c) {}
void inx_sp (cpu *c) {}
void dcx_b (cpu *c) {}
void dcx_d (cpu *c) {}
void dcx_h (cpu *c) {}
void dcx_sp (cpu *c) {}
void xchg (cpu *c) {}
void xhtl (cpu *c) {}
void sphl (cpu *c) {}