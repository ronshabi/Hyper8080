#ifndef HYPER8080_INSTRUCTIONS_H
#define HYPER8080_INSTRUCTIONS_H

#include "defs.h"

void jmp(struct cpu *c, bool condition, uint16_t addr);
void call(struct cpu *c, bool condition, uint16_t addr);
void ret(struct cpu *c, bool condition);

void adi(struct cpu *c); /* ADI - Add immediate to accumulator */
void aci(struct cpu *c); /* ACI - Move Immediate data */
void sui(struct cpu *c); /* SUI - Subtract immediate from accumulator */
void sbi(struct cpu *c); /* SBI - Subtract immediate from accumulator w/ borrow */
void ani(struct cpu *c); /* ANI - And immediate with accumulator */
void xri(struct cpu *c); /* XRI - Xor immediate with accumulator */
void ori(struct cpu *c); /* ORI - Or immediate with accumulator */
void cpi(struct cpu *c); /* CPI - Compare immediate with accumulator */

/* REGISTER PAIR */
void dad_b(struct cpu *c); /* DAD B		Double add */
void dad_d(struct cpu *c); /* DAD D */
void dad_h(struct cpu *c); /* DAD H */
void dad_sp(struct cpu *c);/* DAD SP */
void inx_b(struct cpu *c);/* INX B		Increment register pair */
void inx_d(struct cpu *c);/* INX D */
void inx_h(struct cpu *c);/* INX H */
void inx_sp(struct cpu *c);/* INX SP */
void dcx_b(struct cpu *c);/* DCX B		Decrement register pair */
void dcx_d(struct cpu *c);/* DCX D */
void dcx_h(struct cpu *c);/* DCX H */
void dcx_sp(struct cpu *c);/* DCX SP */
void xchg(struct cpu *c);/* XCHG		Exchange Registers */
void xthl(struct cpu *c);/* XTHL		Exchange Stack */
void sphl(struct cpu *c);/* SPHL		Load SP from H and L */

/* SINGLE REGISTER INSTRUCTIONS */
void inr(struct cpu *c, uint8_t *reg);/* INR	Increment */
void inr_m(struct cpu *c);/* INR M */
void dcr(struct cpu *c, uint8_t *reg);/* DCR	Increment */
void dcr_m(struct cpu *c);/* DCR M */
void daa(struct cpu *c);

/* ROTATE ACCUMULATOR */
void rlc(struct cpu *c);/* RLC Rotate left */
void rrc(struct cpu *c);/* RRC Rotate right */
void ral(struct cpu *c);/* RAR Rotate left through carry */
void rar(struct cpu *c);/* RAR Rotate right through carry */

/* I/O */
void in(struct cpu *c);/* IN */
void out(struct cpu *c);/* OUT */
void hlt(struct cpu *c);/* HALT */

/* REGISTER OR MEMORY TO ACCUMULATOR */
void add(struct cpu *c, const uint8_t *reg);/* ADD A		Add to accumulator */
void add_m(struct cpu *c);/* ADD M */
void adc(struct cpu *c, const uint8_t *reg); /* ADC A		Add to accumulator w/ carry */
void adc_m(struct cpu *c); /* ADC M */
void sub(struct cpu *c, const uint8_t *reg); /* SUB A		/* Subtract from accumulator */
void sub_m(struct cpu *c);/* SUB M */
void sbb(struct cpu *c, const uint8_t *reg); /* SBB A		Subtract from accumulator w/ borrow */
void sbb_m(struct cpu *c);/* SBB M */
void ana(struct cpu *c, const uint8_t *reg);
/* ANA A  	And operation w/ accumulator */

void ana_m(struct cpu *c); /* ANA M */
void xra(struct cpu *c, const uint8_t *reg); /* XRA A		Logical xor w/zero accumulator */
void xra_m(struct cpu *c);/* XRA M */
void ora(struct cpu *c, const uint8_t *reg);/* ORA A		Logical or w/ */
void ora_m(struct cpu *c);/* ORA M */
void cmp(struct cpu *c, const uint8_t *reg); /* CMP A */
void cmp_m(struct cpu *c);/* CMP M */

#endif /* HYPER8080_INSTRUCTIONS_H */