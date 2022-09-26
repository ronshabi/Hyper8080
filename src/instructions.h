/* Created to separate CPU core logic from instructions */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

/* JUMPS */
// https://en.wikipedia.org/wiki/FLAGS_register
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

/* IMMEDIATE INSTRUCTIONS */
void lxi_b (cpu *c);			 // LXI B	Load register pair immediate
void lxi_d (cpu *c);			 // LXI D
void lxi_h (cpu *c);			 // LXI H
void lxi_sp (cpu *c);			 // LXI SP
void mvi (cpu *c, uint8_t *reg); // MVI 	Move immediate data
void mvi_m (cpu *c);			 // MVI M

#endif // INSTRUCTIONS_H
