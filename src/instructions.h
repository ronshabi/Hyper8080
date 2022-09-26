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

#endif // INSTRUCTIONS_H
