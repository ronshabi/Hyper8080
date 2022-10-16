#include "defs.h"

/* CARRY */
void stc (cpu *c) { c->flag_c = 1; }

void cma (cpu *c) { c->a = ~c->a; }

void cmc (cpu *c)
{
	if (c->flag_c == 1) { c->flag_c = 0; }
	else { c->flag_c = 1; }
}

/* JUMP */
void pchl (cpu *c) { c->pc = C_GetHL (c); }
void jmp (cpu *c)
{
#ifdef DEBUG_MODE_REGULAR
	printf (" $%04x", ARG16);
#endif
	c->pc = ARG16;
}

void jmp_addr (cpu *c, uint16_t to)
{
#ifdef DEBUG_MODE_REGULAR
	printf (" $%04x", to);
#endif
	c->pc = to;
}

void jc (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	if (c->flag_c) { jmp_addr (c, to); }
}
void jnc (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	if (!c->flag_c) { jmp_addr (c, to); }
}
void jz (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	if (c->flag_z) { jmp_addr (c, to); }
}
void jnz (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	if (!c->flag_z) { jmp_addr (c, to); }
}
void jm (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	if (c->flag_s) { jmp_addr (c, to); }
}
void jp (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	if (!c->flag_s) { jmp_addr (c, to); }
}
void jpe (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	if (c->flag_p) { jmp_addr (c, to); }
}
void jpo (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	if (!c->flag_p) { jmp_addr (c, to); }
}

/* CALL */
void call (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	S_Push (c, c->pc);
	jmp_addr (c, to);
}

void call_addr (cpu *c, uint16_t addr)
{
	S_Push (c, c->pc);
	jmp_addr (c, addr);
}

void cc (cpu *c)
{
	uint16_t to = ARG16;
	PC2;
	if (c->flag_c) { call_addr (c, to); }
}
void cnc (cpu *c)
{
	uint16_t to = ARG16;
	PC2;

	if (!c->flag_c) { call_addr (c, to); }
}
void cz (cpu *c)
{
	uint16_t to = ARG16;
	PC2;

	if (c->flag_z) { call_addr (c, to); }
}
void cnz (cpu *c)
{
	uint16_t to = ARG16;
	PC2;

	if (!c->flag_z) { call_addr (c, to); }
}
void cm (cpu *c)
{
	uint16_t to = ARG16;
	PC2;

	if (!c->flag_s) { call_addr (c, to); }
}
void cp (cpu *c)
{
	uint16_t to = ARG16;
	PC2;

	if (!c->flag_s) { call_addr (c, to); }
}
void cpe (cpu *c)
{
	uint16_t to = ARG16;
	PC2;

	if (c->flag_p) { call_addr (c, to); }
}
void cpo (cpu *c)
{
	uint16_t to = ARG16;
	PC2;

	if (!c->flag_p) { call_addr (c, to); }
}

/* RET */
void ret (cpu *c)
{
	c->pc = S_Pop (c);
}

void rc (cpu *c)
{
	if (c->flag_c) { ret (c); }
}
void rnc (cpu *c)
{
	if (!c->flag_c) { ret (c); }
}
void rz (cpu *c)
{
	if (c->flag_z) { ret (c); }
}
void rnz (cpu *c)
{
	if (!c->flag_z) { ret (c); }
}
void rm (cpu *c)
{
	if (c->flag_s) { ret (c); }
}
void rp (cpu *c)
{
	if (!c->flag_s) { ret (c); }
}
void rpe (cpu *c)
{
	if (c->flag_p) { ret (c); }
}
void rpo (cpu *c)
{
	if (!c->flag_p) { ret (c); }
}

/* IMMEDIATE C_INSTRUCTIONS */
void lxi_b (cpu *c)
{
	C_SetBC (c, ARG16);
	PC2;
}
void lxi_d (cpu *c)
{
	C_SetDE (c, ARG16);
	PC2;
}
void lxi_h (cpu *c)
{
	C_SetHL (c, ARG16);
	PC2;
}
void lxi_sp (cpu *c)
{
	c->sp = ARG16;
	PC2;
}
void mvi (cpu *c, uint8_t *reg)
{
	*reg = ARG8;
	PC1;
}
void mvi_m (cpu *c)
{
	C_SetByte (c, C_GetHL (c), ARG8);
	PC1;
}
void adi (cpu *c)
{
	uint16_t result = c->a + ARG8;
	c->a			= result & 0xff;
	C_Flags_SetCarryFromWord (c, result);
	C_Flags_SetZSP (c, result);
	PC1;
}
void aci (cpu *c)
{
	uint16_t result = c->a + ARG8 + c->flag_c;
	c->a			= result & 0xff;
	C_Flags_SetCarryFromWord (c, result);
	C_Flags_SetZSP (c, c->a);
	PC1;
}
void sui (cpu *c)
{
	uint16_t result = c->a + FLIP (ARG8);
	c->a			= result & 0xff;
	C_Flags_SetCarryFromWord (c, result);
	C_Flags_SetZSP (c, c->a);
	PC1;
}
void sbi (cpu *c)
{
	uint16_t result = c->a + FLIP (ARG8) + FLIP (c->flag_c);
	c->a			= result & 0xff;
	C_Flags_SetCarryFromWord (c, result);
	C_Flags_SetZSP (c, c->a);
	PC1;
}
void ani (cpu *c)
{
	uint16_t result = c->a & ARG8;
	c->a			= result & 0xff;
	C_Flags_SetCarryFromWord (c, result);
	C_Flags_SetZSP (c, c->a);
	PC1;
}
void xri (cpu *c)
{
	uint16_t result = c->a ^ ARG8;
	c->a			= result & 0xff;
	C_Flags_SetCarryFromWord (c, result);
	C_Flags_SetZSP (c, c->a);
	PC1;
}
void ori (cpu *c)
{
	uint16_t result = c->a | ARG8;
	c->a			= result & 0xff;
	C_Flags_SetCarryFromWord (c, result);
	C_Flags_SetZSP (c, c->a);
	PC1;
}
void cpi (cpu *c)
{
	// A is not changed by this operation, only the FLAGS
	uint16_t result = c->a + FLIP (ARG8);
	C_Flags_SetCarryFromWord (c, result);
	C_Flags_SetZSP (c, result & 0xff);
	PC1;
}

/* DATA TRANSFER */
void ldax_b (cpu *c)
{
	c->a = C_DerefBC (c);
}
void ldax_d (cpu *c) { c->a = C_DerefDE (c); }
void mov (cpu *c, uint8_t *dest, const uint8_t *src) { *dest = *src; }
void mov_m_to_dest (cpu *c, uint8_t *dest)
{
	uint8_t m = C_DerefHL (c);
	*dest	  = m;
}
void mov_m (cpu *c, const uint8_t *reg) { C_SetByte (c, C_GetHL (c), *reg); }
void stax_b (cpu *c) { C_SetByte (c, C_GetBC (c), c->a); }
void stax_d (cpu *c) { C_SetByte (c, C_GetDE (c), c->a); }

/* REGISTER PAIR INSTRUCTIONS */
void push_b (cpu *c) { S_Push (c, C_GetBC (c)); }
void push_d (cpu *c) { S_Push (c, C_GetDE (c)); }
void push_h (cpu *c) { S_Push (c, C_GetHL (c)); }
void push_psw (cpu *c) { S_PushPSW (c); }
void pop_b (cpu *c) { C_SetBC (c, S_Pop (c)); }

void pop_d (cpu *c) { C_SetDE (c, S_Pop (c)); }
void pop_h (cpu *c) { C_SetHL (c, S_Pop (c)); }
void pop_psw (cpu *c) { S_PopPSW (c); }
void dad_b (cpu *c)
{
	uint32_t result = C_GetBC (c) + C_GetHL (c);
	if (result > 0xffff) { c->flag_c = 1; }
	else { c->flag_c = 0; }
	C_SetHL (c, result & 0xffff);
}
void dad_d (cpu *c)
{
	uint32_t result = C_GetDE (c) + C_GetHL (c);
	if (result > 0xffff) { c->flag_c = 1; }
	else { c->flag_c = 0; }
	C_SetHL (c, result & 0xffff);
}
void dad_h (cpu *c)
{
	uint32_t result = C_GetHL (c) + C_GetHL (c);
	if (result > 0xffff) { c->flag_c = 1; }
	else { c->flag_c = 0; }
	C_SetHL (c, result & 0xffff);
}
void dad_sp (cpu *c)
{
	uint32_t result = c->sp + C_GetHL (c);
	if (result > 0xffff) { c->flag_c = 1; }
	else { c->flag_c = 0; }
	C_SetHL (c, result & 0xffff);
}
void inx_b (cpu *c) { C_SetBC (c, C_GetBC (c) + 1); }
void inx_d (cpu *c) { C_SetDE (c, C_GetDE (c) + 1); }
void inx_h (cpu *c) { C_SetHL (c, C_GetHL (c) + 1); }
void inx_sp (cpu *c) { c->sp++; }
void dcx_b (cpu *c) { C_SetBC (c, C_GetBC (c) - 1); }
void dcx_d (cpu *c) { C_SetDE (c, C_GetDE (c) - 1); }
void dcx_h (cpu *c) { C_SetHL (c, C_GetHL (c) - 1); }
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
}
void xthl (cpu *c)
{
	uint8_t l = c->l;
	uint8_t h = c->h;

	c->l = C_DerefSP (c, 0);
	c->h = C_DerefSP (c, 1);

	C_SetByte (c, c->sp, l);
	C_SetByte (c, c->sp + 1, h);
}
void sphl (cpu *c) { c->sp = C_GetHL (c); }

/* SINGLE REGISTER C_INSTRUCTIONS */
void inr (cpu *c, uint8_t *reg)
{
	uint16_t result = ++*reg;
	C_Flags_SetZSP (c, *reg);
	C_Flags_SetCarryFromWord (c, result);
	*reg = result & 0xff;
}
void inr_m (cpu *c)
{
	uint16_t result = C_DerefHL (c) + 1;
	C_SetByte (c, C_GetHL (c), result & 0xff);
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
}
void dcr (cpu *c, uint8_t *reg)
{
	uint16_t result = *reg + FLIP (1);
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result & 0xff);
	*reg = result & 0xff;

#ifdef DEBUG_MODE_REGULAR
	if (*reg == 0) { printf ("\nREG HIT ZERO\n"); }
#endif
}
void dcr_m (cpu *c)
{
	uint16_t result = C_DerefHL (c) + FLIP (1);
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result & 0xff);
	C_SetByte (c, C_GetHL (c), result & 0xff);

#ifdef DEBUG_MODE_REGULAR
	if (C_DerefHL (c) == 0) { printf ("\nM HIT ZERO\n"); }
#endif
}
void daa (cpu *c)
{
	uint16_t result = 0;
	if ((c->a & 0x0f) > 9 || c->flag_ac) { result += 6; }

	if (((c->a >> 4) > 9) || (c->flag_c))
	{
		result += 0x60;
		c->flag_c = 1;
		result += c->a;
		C_Flags_SetZSP (c, result & 0xff);
		C_Flags_SetCarryFromWord (c, result);
	}
}

/* ROTATE ACCUMULATOR */
void rlc (cpu *c)
{
	uint8_t bit7 = (c->a & 0x80) >> 7;
	c->a <<= 1;
	c->a |= bit7;
	c->flag_c = bit7;
}
void rrc (cpu *c)
{
	uint8_t bit0 = (c->a & 0x1);
	c->a >>= 1;
	c->a |= bit0 << 7;
	c->flag_c = bit0;
}
void ral (cpu *c)
{
	uint8_t bit7 = (c->a & 0x80) >> 7;
	c->a <<= 1;
	c->a |= c->flag_c;
	c->flag_c = bit7;
}
void rar (cpu *c)
{
	uint8_t bit0 = c->a & 0x1;
	c->a		 = c->a >> 1;
	c->a		 = c->a | (c->flag_c << 7);
	c->flag_c	 = bit0;
}

/* I/O */
void in (cpu *c)
{
	uint8_t device_number = ARG8;
	PC1;

#ifdef DEBUG_MODE_REGULAR
	printf (" <DEVICE = %d>", device_number);
#endif

	if (device_number == DEVICE_INP0) { c->a = c->i0; }
	else if (device_number == DEVICE_INP1) { c->a = c->i1; }
	else if (device_number == DEVICE_INP2) { c->a = c->i2; }
	else if (device_number == DEVICE_SHIFT_IN) { c->a = (c->shift >> (8 - c->shift_amt)); }
}
void out (cpu *c)
{
	uint8_t device_number = ARG8;
	PC1;

#ifdef DEBUG_MODE_REGULAR
	printf (" <DEVICE = %d>", device_number);
#endif
	if (device_number == DEVICE_SHIFT_AMT) { c->shift_amt = c->a & 0x7; }
	else if (device_number == DEVICE_SOUND1) { c->o3 = c->a; }
	else if (device_number == DEVICE_SHIFT_DATA)
	{
		c->shift >>= 8;
		c->shift |= c->a << 8;
	}
	else if (device_number == DEVICE_SOUND2) { c->o5 = c->a; }
	else if (device_number == DEVICE_WATCHDOG) { c->o6 = c->a; }
}
void hlt (cpu *c)
{
#ifdef DEBUG_MODE_REGULAR
	printf ("\nHALTED!\n");
#endif
	c->halt = 1;
}

/* REGISTER OR MEMORY TO ACCUMULATOR C_INSTRUCTIONS */
void add (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + *reg;
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void add_m (cpu *c)
{
	uint16_t result = c->a + C_DerefHL (c);
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void adc (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + *reg + c->flag_c;
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void adc_m (cpu *c)
{
	uint16_t result = c->a + C_DerefHL (c) + c->flag_c;
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void sub (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP (*reg);
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void sub_m (cpu *c)
{
	uint16_t result = c->a + FLIP (C_DerefHL (c));
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void sbb (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a + FLIP (*reg) + FLIP (c->flag_c);
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void sbb_m (cpu *c)
{
	uint16_t result = c->a + FLIP (C_DerefHL (c)) + FLIP (c->flag_c);
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void ana (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a & *reg;
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void ana_m (cpu *c)
{
	uint16_t result = c->a & (C_DerefHL (c));
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void xra (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a ^ *reg;
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void xra_m (cpu *c)
{
	uint16_t result = c->a ^ (C_DerefHL (c));
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void ora (cpu *c, const uint8_t *reg)
{
	uint16_t result = c->a | *reg;
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void ora_m (cpu *c)
{
	uint16_t result = c->a | (C_DerefHL (c));
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
	c->a = result & 0xff;
}
void cmp (cpu *c, uint8_t *r)
{
	uint16_t result = c->a - *r;
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
}

void cmp_m (cpu *c)
{
	uint16_t result = c->a - C_DerefHL (c);
	C_Flags_SetZSP (c, result & 0xff);
	C_Flags_SetCarryFromWord (c, result);
}

/* DIRECT ADDRESSING C_INSTRUCTIONS */
void sta (cpu *c)
{
	uint16_t adr = ARG16;
	C_SetByte (c, adr, c->a);
	PC2;
}
void lda (cpu *c)
{
	uint16_t adr = ARG16;
#ifdef DEBUG_MODE_REGULAR
	printf (" $%04x", adr);
#endif
	c->a = C_GetByte (c, adr);

	PC2;
}
void shld (cpu *c)
{
	uint16_t adr = ARG16;
	C_SetWord (c, adr, C_GetHL (c));
	PC2;
}
void lhld (cpu *c)
{
	uint16_t adr  = ARG16;
	uint16_t word = C_GetWord (c, adr);
	C_SetHL (c, word);
	PC2;
}

/* INTERRUPT C_INSTRUCTIONS */
void set_interrupt (cpu *c, uint8_t state)
{
	c->interrupts_enabled = state;
}
