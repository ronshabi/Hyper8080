#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct cpu
{
	uint8_t a, b, c, d, e, h, l;
	uint16_t pc, sp;

	uint8_t *memory;

	// condition
	uint64_t cycle_count;
	uint8_t halt, interrupts_enabled;

	// flags
	uint8_t flag_z, flag_s, flag_p, flag_c, flag_ac;

	// iX - input port X, oX - output port X
	uint8_t i0, i1, i2, i3, o2, o3, o4, o5, o6;

	// bitshift register
	uint16_t shift;
	uint8_t shift_amt;

} cpu;

void cpu_init(cpu *c);
void cpu_set_memory(cpu *c, uint8_t *memory_ptr);

uint8_t cpu_get_byte(cpu *c, uint16_t address);
uint16_t cpu_get_word(cpu *c, uint16_t address);
void cpu_set_byte(cpu *c, uint16_t address, uint8_t val);
void cpu_set_word(cpu *c, uint16_t address, uint16_t val);

uint16_t cpu_get_bc(cpu *c);
uint16_t cpu_get_de(cpu *c);
uint16_t cpu_get_hl(cpu *c);

void cpu_set_bc(cpu *c, uint16_t val);
void cpu_set_de(cpu *c, uint16_t val);
void cpu_set_hl(cpu *c, uint16_t val);

uint16_t cpu_deref_bc(cpu *c);
uint16_t cpu_deref_de(cpu *c);
uint16_t cpu_deref_hl(cpu *c);

void cpu_stack_push(cpu *c, uint16_t val);
uint16_t cpu_stack_pop(cpu *c);

uint8_t cpu_get_flags(cpu *c);
void cpu_set_flags_zsp(cpu *c, uint8_t val);
void cpu_set_flags_c(cpu *c, uint8_t f, uint8_t g, uint8_t modulator);
void cpu_set_flags_ac(cpu *c, uint8_t f, uint8_t g, uint8_t modulator);
void cpu_set_flags_all(cpu *c, uint8_t f, uint8_t g, uint8_t modulator);

#endif // CPU_H