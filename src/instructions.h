#ifndef HYPER8080_INSTRUCTIONS_H
#define HYPER8080_INSTRUCTIONS_H

#include "defines.h"

void inst_jmp(struct cpu *c, bool condition, uint16_t addr);
void inst_call(struct cpu *c, bool condition, uint16_t addr);
void inst_ret(struct cpu *c, bool condition);

/* SINGLE REGISTER INSTRUCTIONS */
void inr(struct cpu *c, uint8_t *reg);/* INR	Increment */
void dcr(struct cpu *c, uint8_t *reg);/* DCR	Increment */

/* REGISTER OR MEMORY TO ACCUMULATOR */
void add(struct cpu *c, const uint8_t *reg);/* ADD A		Add to accumulator */
void adc(struct cpu *c, const uint8_t *reg); /* ADC A		Add to accumulator w/ carry */
void sub(struct cpu *c, const uint8_t *reg); /* SUB A		/* Subtract from accumulator */
void sub_m(struct cpu *c);/* SUB M */
void sbb(struct cpu *c, const uint8_t *reg); /* SBB A		Subtract from accumulator w/ borrow */
void sbb_m(struct cpu *c);/* SBB M */
void ana(struct cpu *c, const uint8_t *reg);
/* ANA A  	And operation w/ accumulator */

void ana_m(struct cpu *c); /* ANA M */
void xra(struct cpu *c, const uint8_t *reg); /* XRA A Logical xor zero accumulator */
void ora(struct cpu *c, const uint8_t *reg);/* ORA */
void cmp(struct cpu *c, const uint8_t *reg); /* CMP */

#endif /* HYPER8080_INSTRUCTIONS_H */