#ifndef HYPER8080_CPU_H
#define HYPER8080_CPU_H

#define PC1 c->pc += 1
#define PC2 c->pc += 2

/* Quickly access register location */
#define REG(reg) &(c->reg)

/* Quickly get immediate */
#define ARG8 cpu_get_byte(c, c->pc)
#define ARG16 cpu_get_word(c, c->pc)

/* Twos-compliment flip sign */
#define FLIP(n) (~(n) + 1)

/* Device numbers */
/* Output */
#define DEVICE_SHIFT_AMT 2
#define DEVICE_SOUND1 3
#define DEVICE_SHIFT_DATA 4
#define DEVICE_SOUND2 5
#define DEVICE_WATCHDOG 6

/* Input */
#define DEVICE_INP0 0
#define DEVICE_INP1 1
#define DEVICE_INP2 2
#define DEVICE_SHIFT_IN 3

struct cpu {
	uint8_t a, b, c, d, e, h, l;
	uint16_t pc, sp;
	uint8_t *memory;

	/* condition */
	uint64_t cycles, instructions;
	bool halt, interrupts_enabled;

	/* flags */
	bool flag_z, flag_s, flag_p, flag_c, flag_ac;

	/* iX - input port X, oX - output port X */
	uint8_t i0, i1, i2, i3, o2, o3, o4, o5, o6;

	/* bitshift register */
	uint16_t shift;
	uint8_t shift_amt;

	/* debugging tools */
	uint8_t paused;

};

void 		cpu_init(struct cpu *);
void 		cpu_set_memory(struct cpu *, uint8_t *);
uint8_t 	cpu_get_byte(struct cpu *, uint16_t);
uint16_t 	cpu_get_word(struct cpu *, uint16_t);
void 		cpu_set_byte(struct cpu *, uint16_t, uint8_t);
void 		cpu_set_word(struct cpu *, uint16_t, uint16_t);
uint16_t 	cpu_get_bc(struct cpu *);
uint16_t 	cpu_get_de(struct cpu *);
uint16_t 	cpu_get_hl(struct cpu *);
void 		cpu_set_bc(struct cpu *, uint16_t);
void 		cpu_set_de(struct cpu *, uint16_t);
void 		cpu_set_hl(struct cpu *, uint16_t);
uint8_t 	cpu_deref_bc(struct cpu *);
uint8_t 	cpu_deref_de(struct cpu *);
uint8_t 	cpu_deref_hl(struct cpu *);
uint8_t 	cpu_deref_sp(struct cpu *, uint16_t);
void 		cpu_flags_set_zsp(struct cpu *, uint8_t);
void 		cpu_flags_set_carry_from_word(struct cpu *, uint16_t);

void 		cpu_stack_push(struct cpu *, uint16_t);
uint16_t 	cpu_stack_pop(struct cpu *);
void 		cpu_stack_push_psw(struct cpu *);
void 		cpu_stack_pop_psw(struct cpu *);

void cpu_execute(struct cpu *, uint8_t);
void cpu_unimplemented(struct cpu *);
void cpu_interrupt(struct cpu *, uint8_t);

#endif /* HYPER8080_CPU_H */