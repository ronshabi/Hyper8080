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

/* IMMEDIATE INSTRUCTIONS */
void lxi_b (cpu *c);			 // LXI B	Load register pair immediate
void lxi_d (cpu *c);			 // LXI D
void lxi_h (cpu *c);			 // LXI H
void lxi_sp (cpu *c);			 // LXI SP
void mvi (cpu *c, uint8_t *reg); // MVI 	Move immediate data
void mvi_m (cpu *c);			 // MVI M

/* DATA TRANSFER */
void ldax_b (cpu *c); // LDAX B		Load accumulator
void ldax_d (cpu *c); // LDAX D
void mov_m (cpu *c, const uint8_t *reg);

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
void xthl (cpu *c);		// XHTL		Exchange Stack
void sphl (cpu *c);		// SPHL		Load SP from H and L

#endif // INSTRUCTIONS_H
