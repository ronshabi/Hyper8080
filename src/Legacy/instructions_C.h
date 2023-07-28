#pragma once

#include "defs.h"

void jmp(cpu *c, bool condition, uint16_t addr);

void call(cpu *c, bool condition, uint16_t addr);

void ret(cpu *c, bool condition);

void adi(cpu *c); // ADI - Add immediate to accumulator
void aci(cpu *c); // ACI - Move Immediate data
void sui(cpu *c); // SUI - Subtract immediate from accumulator
void sbi(cpu *c); // SBI - Subtract immediate from accumulator w/ borrow
void ani(cpu *c); // ANI - And immediate with accumulator
void xri(cpu *c); // XRI - Xor immediate with accumulator
void ori(cpu *c); // ORI - Or immediate with accumulator
void cpi(cpu *c); // CPI - Compare immediate with accumulator

/* REGISTER PAIR */
void dad_b(cpu *c);  // DAD B		Double add
void dad_d(cpu *c);  // DAD D
void dad_h(cpu *c);  // DAD H
void dad_sp(cpu *c); // DAD SP
void inx_b(cpu *c);  // INX B		Increment register pair
void inx_d(cpu *c);  // INX D
void inx_h(cpu *c);  // INX H
void inx_sp(cpu *c); // INX SP
void dcx_b(cpu *c);  // DCX B		Decrement register pair
void dcx_d(cpu *c);  // DCX D
void dcx_h(cpu *c);  // DCX H
void dcx_sp(cpu *c); // DCX SP
void xchg(cpu *c);   // XCHG		Exchange Registers
void xthl(cpu *c);   // XTHL		Exchange Stack
void sphl(cpu *c);   // SPHL		Load SP from H and L

/* SINGLE REGISTER INSTRUCTIONS */
void inr(cpu *c, uint8_t *reg); // INR	Increment
void inr_m(cpu *c);             // INR M
void dcr(cpu *c, uint8_t *reg); // DCR	Increment
void dcr_m(cpu *c);             // DCR M
void daa(cpu *c);

/* ROTATE ACCUMULATOR */
void rlc(cpu *c); // RLC Rotate left
void rrc(cpu *c); // RRC Rotate right
void ral(cpu *c); // RAR Rotate left through carry
void rar(cpu *c); // RAR Rotate right through carry

/* I/O */
void in(cpu *c);  // IN
void out(cpu *c); // OUT
void hlt(cpu *c); // HALT

/* REGISTER OR MEMORY TO ACCUMULATOR */
void add(cpu *c, const uint8_t *reg); // ADD A		Add to accumulator
void add_m(cpu *c);                   // ADD M
void adc(cpu *c,
         const uint8_t *reg);         // ADC A		Add to accumulator w/ carry
void adc_m(cpu *c);                   // ADC M
void sub(cpu *c, const uint8_t *reg); // SUB A		Subtract from
// accumulator
void sub_m(cpu *c);                   // SUB M
void sbb(cpu *c,
         const uint8_t
         *reg); // SBB A		Subtract from accumulator w/ borrow
void sbb_m(cpu *c); // SBB M
void ana(cpu *c,
         const uint8_t *reg); // ANA A  	And operation w/ accumulator
void ana_m(cpu *c);           // ANA M
void xra(cpu *c,
         const uint8_t *reg);         // XRA A		Logical xor w/zero accumulator
void xra_m(cpu *c);                   // XRA M
void ora(cpu *c, const uint8_t *reg); // ORA A		Logical or w/
// accumulator
void ora_m(cpu *c);                   // ORA M
void cmp(cpu *c, const uint8_t *reg); // CMP A
void cmp_m(cpu *c);                   // CMP M