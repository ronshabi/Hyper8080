#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define PC1 c->pc += 1
#define PC2 c->pc += 2
#define PC3 c->pc += 3

// Quickly access register location
#define REG(reg) &(c->reg)

// Quickly get immediate
#define GET_IMMEDIATE_BYTE C_GetByte (c, c->pc + 1)

#define FLIP(n) (~n + 1)

// Device numbers
// Output
#define DEVICE_SHIFT_AMT  2
#define DEVICE_SOUND1	  3
#define DEVICE_SHIFT_DATA 4
#define DEVICE_SOUND2	  5
#define DEVICE_WATCHDOG	  6

// Input
#define DEVICE_INP0		0
#define DEVICE_INP1		1
#define DEVICE_INP2		2
#define DEVICE_SHIFT_IN 3

typedef struct cpu
{
	uint8_t	 a, b, c, d, e, h, l;
	uint16_t pc, sp;
	uint8_t *memory;

	// condition
	uint64_t cycles, instructions;
	uint8_t	 halt, interrupts_enabled;

	// flags
	uint8_t flag_z, flag_s, flag_p, flag_c, flag_ac;

	// iX - input port X, oX - output port X
	uint8_t i0, i1, i2, i3, o2, o3, o4, o5, o6;

	// bitshift register
	uint16_t shift;
	uint8_t	 shift_amt;

} cpu;

// Core
void cpu_init (cpu *c);
void C_SetMemory (cpu *c, uint8_t *memory_ptr);
void C_DisAsm (cpu *c);

// Memory
uint8_t	 C_GetByte (cpu *c, uint16_t address);
uint16_t cpu_get_word (cpu *c, uint16_t address);
void	 C_SetByte (cpu *c, uint16_t address, uint8_t val);
void	 cpu_set_word (cpu *c, uint16_t address, uint16_t val);

// Register pairs
uint16_t C_GetBC (cpu *c);
uint16_t C_GetDE (cpu *c);
uint16_t C_GetHL (cpu *c);
void	 C_SetBC (cpu *c, uint16_t val);
void	 C_SetDE (cpu *c, uint16_t val);
void	 C_SetHL (cpu *c, uint16_t val);
uint16_t C_DerefBC (cpu *c);
uint16_t C_DerefDE (cpu *c);
uint16_t C_DerefHL (cpu *c);
uint16_t C_DerefSP (cpu *c, uint16_t offset);

// Flags
uint8_t cpu_get_flags (cpu *c);
void	cpu_set_flags_zsp (cpu *c, uint8_t val);
void	cpu_set_flags_carry_add (cpu *c, uint8_t a, uint8_t b, uint8_t carry);
void	cpu_set_flags_carry_from_16bit (cpu *c, uint16_t num);

// Stack
void	 stack_push (cpu *c, uint16_t val);
uint16_t stack_pop (cpu *c);
void	 stack_push_psw (cpu *c);
void	 stack_pop_psw (cpu *c);

// Emulation
void C_Emulate (cpu *c, uint8_t opcode);
void C_Unimplemented (cpu *c);
void C_GenerateInterrupt (cpu *c, int intnum);

#endif // CPU_H