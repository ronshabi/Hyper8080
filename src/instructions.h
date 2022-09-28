/* Created to separate CPU core logic from instructions */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

// https://en.wikipedia.org/wiki/FLAGS_register

/* JUMP */
void pchl (cpu *c); // PCHL		Load program counter
void jmp (cpu *c);	// JMP		Jump
void jc (cpu *c);	// JC		JMP carry
void jnc (cpu *c);	// JNC		JMP not carry
void jz (cpu *c);	// JZ		JMP zero
void jnz (cpu *c);	// JNZ		JMP not zero
void jm (cpu *c);	// JM		JMP minus
void jp (cpu *c);	// JP		JMP positive
void jpe (cpu *c);	// JPE		JMP parity even
void jpo (cpu *c);	// JPO		JMP parity odd

/* CALL */
void call (cpu *c); // CALL
void cc (cpu *c);	// CC       CALL carry
void cnc (cpu *c);	// CNC      CALL not carry
void cz (cpu *c);	// CZ       CALL zero
void cnz (cpu *c);	// CNZ      CALL not zero
void cm (cpu *c);	// CM       CALL minus
void cp (cpu *c);	// CP       CALL positive
void cpe (cpu *c);	// CPE      CALL parity even
void cpo (cpu *c);	// CPO      CALL parity odd

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

/* IMMEDIATE INSTRUCTIONS */
void lxi_b (cpu *c);			 // LXI B	Load register pair immediate
void lxi_d (cpu *c);			 // LXI D
void lxi_h (cpu *c);			 // LXI H
void lxi_sp (cpu *c);			 // LXI SP
void mvi (cpu *c, uint8_t *reg); // MVI 	Move immediate data
void mvi_m (cpu *c);			 // MVI M

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

/* REGISTER PAIR INSTRUCTIONS */
void push_b (cpu *c);	// PUSH B
void push_d (cpu *c);	// PUSH D
void push_h (cpu *c);	// PUSH H
void push_psw (cpu *c); // PUSH PSW
void pop_b (cpu *c);	// POP B
void pop_d (cpu *c);	// POP D
void pop_h (cpu *c);	// POP H
void pop_psw (cpu *c);	// POP PSW
void dad_b (cpu *c);	// DAD B		Double add
void dad_d (cpu *c);	// DAD D
void dad_h (cpu *c);	// DAD H
void dad_sp (cpu *c);	// DAD SP
void inx_b (cpu *c);	// INX B		Increment register pair
void inx_d (cpu *c);	// INX D
void inx_h (cpu *c);	// INX H
void inx_sp (cpu *c);	// INX SP
void dcx_b (cpu *c);	// DCX B		Decrement register pair
void dcx_d (cpu *c);	// DCX D
void dcx_h (cpu *c);	// DCX H
void dcx_sp (cpu *c);	// DCX SP
void xchg (cpu *c);		// XCHG		Exchange Registers
void xthl (cpu *c);		// XTHL		Exchange Stack
void sphl (cpu *c);		// SPHL		Load SP from H and L

/* SINGLE REGISTER INSTRUCTIONS */
void inr (cpu *c, uint8_t *reg); // INR	Increment
void inr_m (cpu *c);			 // INR M
void dcr (cpu *c, uint8_t *reg); // DCR	Increment
void dcr_m (cpu *c);			 // DCR M

/* ROTATE ACCUMULATOR */
void rlc (cpu *c); // RLC Rotate left
void rrc (cpu *c); // RRC Rotate right
void ral (cpu *c); // RAR Rotate left through carry
void rar (cpu *c); // RAR Rotate right through carry

/* I/O */
void in (cpu *c);  // IN
void out (cpu *c); // OUT
void hlt (cpu *c); // HALT

/* REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS */
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

/* DIRECT ADDRESSING INSTRUCTIONS */
void sta (cpu *c);	// STA		Store accumulator direct
void lda (cpu *c);	// LDA		Load accumulator direct
void shld (cpu *c); // SHLD		Store H and L direct
void lhld (cpu *c); // LHLD		Load H and L direct

#endif // INSTRUCTIONS_H
