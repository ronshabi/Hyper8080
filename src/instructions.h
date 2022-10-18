/* Created to separate CPU core logic from instructions */

#pragma once
#include "defs.h"

// https://en.wikipedia.org/wiki/FLAGS_register

/* CARRY */

/* JUMP */
void jmp (cpu *c, bool condition, uint16_t addr); // JMP		Jump

/* CALL */
void call_addr (cpu *c, uint16_t addr);
void call (cpu *c, bool condition, uint16_t addr); // CALL

/* RET */
void ret (cpu *c); // RET
void rc (cpu *c);  // RC       RET carry
void rnc (cpu *c); // RNC      RET not carry
void rz (cpu *c);  // RZ       RET zero
void rnz (cpu *c); // RNZ      RET not zero
void rm (cpu *c);  // RM       RET minus
void rp (cpu *c);  // RP       RET positive
void rpe (cpu *c); // RPE      RET parity even
void rpo (cpu *c); // RPO      RET parity odd

/* IMMEDIATE C_INSTRUCTIONS */
void lxi_b (cpu *c);  // LXI B	Load register pair immediate
void lxi_d (cpu *c);  // LXI D
void lxi_h (cpu *c);  // LXI H
void lxi_sp (cpu *c); // LXI SP

void adi (cpu *c); // ADI - Add immediate to accumulator
void aci (cpu *c); // ACI - Move Immediate data
void sui (cpu *c); // SUI - Subtract immediate from accumulator
void sbi (cpu *c); // SBI - Subtract immediate from accumulator w/ borrow
void ani (cpu *c); // ANI - And immediate with accumulator
void xri (cpu *c); // XRI - Xor immediate with accumulator
void ori (cpu *c); // ORI - Or immediate with accumulator
void cpi (cpu *c); // CPI - Compare immediate with accumulator

/* DATA TRANSFER */
void ldax_b (cpu *c); // LDAX B		Load accumulator
void ldax_d (cpu *c); // LDAX D
void mov (cpu *c, uint8_t *dest, const uint8_t *src);
void mov_m_to_dest (cpu *c, uint8_t *dest);
void mov_m (cpu *c, const uint8_t *reg);
void stax_b (cpu *c);
void stax_d (cpu *c);

/* REGISTER PAIR */
void dad_b (cpu *c);  // DAD B		Double add
void dad_d (cpu *c);  // DAD D
void dad_h (cpu *c);  // DAD H
void dad_sp (cpu *c); // DAD SP
void inx_b (cpu *c);  // INX B		Increment register pair
void inx_d (cpu *c);  // INX D
void inx_h (cpu *c);  // INX H
void inx_sp (cpu *c); // INX SP
void dcx_b (cpu *c);  // DCX B		Decrement register pair
void dcx_d (cpu *c);  // DCX D
void dcx_h (cpu *c);  // DCX H
void dcx_sp (cpu *c); // DCX SP
void xchg (cpu *c);	  // XCHG		Exchange Registers
void xthl (cpu *c);	  // XTHL		Exchange Stack
void sphl (cpu *c);	  // SPHL		Load SP from H and L

/* SINGLE REGISTER C_INSTRUCTIONS */
void inr (cpu *c, uint8_t *reg); // INR	Increment
void inr_m (cpu *c);			 // INR M
void dcr (cpu *c, uint8_t *reg); // DCR	Increment
void dcr_m (cpu *c);			 // DCR M
void daa (cpu *c);

/* ROTATE ACCUMULATOR */
void rlc (cpu *c); // RLC Rotate left
void rrc (cpu *c); // RRC Rotate right
void ral (cpu *c); // RAR Rotate left through carry
void rar (cpu *c); // RAR Rotate right through carry

/* I/O */
void in (cpu *c);  // IN
void out (cpu *c); // OUT
void hlt (cpu *c); // HALT

/* REGISTER OR MEMORY TO ACCUMULATOR C_INSTRUCTIONS */
void add (cpu *c, const uint8_t *reg); // ADD A		Add to accumulator
void add_m (cpu *c);				   // ADD M
void adc (cpu *c, const uint8_t *reg); // ADC A		Add to accumulator w/ carry
void adc_m (cpu *c);				   // ADC M
void sub (cpu *c, const uint8_t *reg); // SUB A		Subtract from accumulator
void sub_m (cpu *c);				   // SUB M
void sbb (cpu *c, const uint8_t *reg); // SBB A		Subtract from accumulator w/ borrow
void sbb_m (cpu *c);				   // SBB M
void ana (cpu *c, const uint8_t *reg); // ANA A  		And operation w/ accumulator
void ana_m (cpu *c);				   // ANA M
void xra (cpu *c, const uint8_t *reg); // XRA A		Logical xor w/zero accumulator
void xra_m (cpu *c);				   // XRA M
void ora (cpu *c, const uint8_t *reg); // ORA A		Logical or w/ accumulator
void ora_m (cpu *c);				   // ORA M
void cmp (cpu *c, const uint8_t *reg); // CMP A
void cmp_m (cpu *c);				   // CMP M

/* DIRECT ADDRESSING C_INSTRUCTIONS */
void sta (cpu *c);	// STA		Store accumulator direct
void lda (cpu *c);	// LDA		Load accumulator direct
void shld (cpu *c); // SHLD		Store H and L direct
void lhld (cpu *c); // LHLD		Load H and L direct

/* INTERRUPT C_INSTRUCTIONS */
void set_interrupt (cpu *c, uint8_t state);
