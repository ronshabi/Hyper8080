/*
 * Hyper8080 / CPU.c
 * 
 * Copyright (c) 2022 Ron Shabi
 */

#include "defines.h"

/*
 * Returns true if number of ones in byte is even.
 */
bool
parity(uint8_t n)
{
	uint8_t parity = 0;

	while (n) {
		parity ^= n & 1;
		n >>= 1;
	}

	return 1 - parity;
}

/*
 * Initialise CPU state.
 * 
 * Note: this function sets the memory pointer to NULL, therefore it shouldn't
 * be called before freeing unused allocated memory.
 */
void
cpu_init(struct cpu *c)
{
	c->a = 0;
	c->b = 0;
	c->c = 0;
	c->d = 0;
	c->e = 0;
	c->h = 0;
	c->l = 0;

	c->flag_z = 0;
	c->flag_s = 0;
	c->flag_p = 0;
	c->flag_c = 0;
	c->flag_ac = 0;

	c->pc = 0;
	c->sp = 0;

	c->cycles = 0;
	c->instructions = 0;

	c->memory = NULL;
	c->halt = 0;
	c->interrupts_enabled = 0;

	c->i0 = 0;
	c->i1 = 0;
	c->i2 = 0;
	c->i3 = 0;
	c->o2 = 0;
	c->o3 = 0;
	c->o4 = 0;
	c->o5 = 0;
	c->o6 = 0;

	c->shift = 0;
	c->shift_amt = 0;

	c->paused = 0;
}

/*
 * Point memory to address at ptr
 */
void
cpu_set_memory(struct cpu *c, uint8_t *ptr)
{
	c->memory = ptr;
}

/*
 * Get byte at offset from memory
 */
uint8_t
cpu_get_byte(struct cpu *c, uint16_t offset)
{
	return c->memory[offset];
}

/*
 * Set byte at offset in memory.
 *
 * Note: this function applys ROM write protection, currently set to be
 * in memory addresses between 0x2000 - 0x4000, in order to prevent memory
 * corruptions which should not happen in real hardware.
 */
void
cpu_set_byte(struct cpu *c, uint16_t offset, uint8_t val)
{
	if (offset >= 0x2000 && offset <= 0x4000)
		c->memory[offset] = val;
}

/*
 * Get word at offset from memory as a 16-bit LE uint.
 */
uint16_t
cpu_get_word(struct cpu *c, uint16_t offset)
{
	return c->memory[offset + 1] << 8 | c->memory[offset];
}

/*
 * Set word at offset in memory in LE format.
 *
 * Note: as with cpu_set_byte, this function implements ROM write protection.
 */
void
cpu_set_word(struct cpu *c, uint16_t address, uint16_t val)
{
	cpu_set_byte(c, address, val & 0xff);
	cpu_set_byte(c, address + 1, val >> 8);
}

/*
 * Get 16-bit uint assembled from register pairs BC, DE, HL.
 */
uint16_t
cpu_get_bc(struct cpu *c)
{
	return c->b << 8 | c->c;
}

uint16_t
cpu_get_de(struct cpu *c)
{
	return c->d << 8 | c->e;
}

uint16_t
cpu_get_hl(struct cpu *c)
{
	return c->h << 8 | c->l;
}

/*
 * Get 16-bit PSW register as defined in the manual:
 * register A occupies most significant 8-bits, while the FLAGS register
 * occupies the least significant 8-bits.
 */
uint16_t
cpu_get_psw(struct cpu *c)
{
	uint16_t ret;
	ret = 0;

	ret |= c->a << 8;

	ret |= c->flag_s << 7;
	ret |= c->flag_z << 6;
	ret |= 0 << 5;
	ret |= c->flag_ac << 4;
	ret |= 0 << 3;
	ret |= c->flag_p << 2;
	ret |= 1 << 1;
	ret |= c->flag_c;

	return ret;
}

/*
 * Set register pairs BC, DE, HL to immediate word given as argument.
 */
void
cpu_set_bc(struct cpu *c, uint16_t word)
{
	c->b = word >> 8;
	c->c = word & 0xff;
}
void
cpu_set_de(struct cpu *c, uint16_t word)
{
	c->d = word >> 8;
	c->e = word & 0xff;
}
void
cpu_set_hl(struct cpu *c, uint16_t word)
{
	c->h = word >> 8;
	c->l = word & 0xff;
}

/*
 * Get byte at memory offset assembled by register pairs BC, DE, HL.
 */
uint8_t
cpu_deref_bc(struct cpu *c)
{
	return cpu_get_byte(c, cpu_get_bc(c));
}

uint8_t
cpu_deref_de(struct cpu *c)
{
	return cpu_get_byte(c, cpu_get_de(c));
}

uint8_t
cpu_deref_hl(struct cpu *c)
{
	return cpu_get_byte(c, cpu_get_hl(c));
}

/*
 * Get byte at memory offset denoted in register SP with an offset given as
 * an additional argument.
 */
uint8_t
cpu_deref_sp(struct cpu *c, uint16_t offset)
{
	return cpu_get_byte(c, c->sp + offset);
}

/*
 * Push 16-bit uint (word) to stack.
 */
void
cpu_stack_push(struct cpu *c, uint16_t word)
{
	c->sp -= 2;
	cpu_set_word(c, c->sp, word);
}

/*
 * Pop word from stack, return it to user as 16-bit uint.
 */
uint16_t
cpu_stack_pop(struct cpu *c)
{
	uint16_t ret = cpu_get_word(c, c->sp);
	c->sp += 2;
	return ret;
}

/* 
 * Push value of PSW register to the stack.
 */
void
cpu_stack_push_psw(struct cpu *c)
{
	cpu_stack_push(c, cpu_get_psw(c));
}

/* 
 * Pop value of PSW register from the stack, set A and FLAGS registers to
 * values denoted in it.
 */
void
cpu_stack_pop_psw(struct cpu *c)
{
	uint16_t psw;

	psw = cpu_stack_pop(c);

	c->a = psw >> 8;
	c->flag_s = psw >> 7 & 0x1;
	c->flag_z = psw >> 6 & 0x1;
	c->flag_ac = psw >> 4 & 0x1;
	c->flag_p = psw >> 2 & 0x1;
	c->flag_c = psw & 0x1;
}


/* 
 * Set flags Zero, Sign and Parity based on provided number.
 */
void
cpu_flags_set_zsp(struct cpu *c, uint8_t n)
{
	c->flag_z = n == 0;
	c->flag_s = (n & 0x80) == 0x80;
	c->flag_p = parity(n);
}

/* 
 * Set Carry flag based on provided 16-bit uint.
 */
void
cpu_flags_set_carry_from_word(struct cpu *c, uint16_t word)
{
	c->flag_c = word > 0xff;
}

/* 
 * Prints error about an unimplemented instruction to stderr with the
 * instruction's opcode, then exit. 
 */
void
cpu_unimplemented(struct cpu *c)
{
	fprintf(stderr, 
		"Error: unimplemented instruction %02x\n",
		cpu_get_byte(c, --c->pc));

	exit(1);
}

/*
 * Generate interrupt (asm INT)
 */
void
cpu_interrupt(struct cpu *c, uint8_t intnum)
{
	if (c->interrupts_enabled) {
		c->interrupts_enabled = 0;
		cpu_stack_push(c, c->pc);
		c->pc = intnum*8;
	}
}

/*
 * Execute an instruction based on opcode provided in argument.
 *
 * While most instruction opcodes are based on the manual, some undocumented
 * instructions are necessary to implement to ensure correct emulation (for
 * reference, see https://pastraiser.com/cpu/i8080/i8080_opcodes.html)
 * 
 * For each instruction, increment the CPU instructions counter and add the
 * cycles required by the executed instruction to the CPU cycle counter.
 * 
 * If the opcode provided isn't associated with any 8080 function, this
 * function will call the _unimplemented function_ handler (in this case:
 * cpu_unimplemented).
 */
void
cpu_execute(struct cpu *c, uint8_t opcode)
{
	uint32_t tmp16;		/* Temporary u16 for calculations */
	uint32_t tmp32;		/* Temporary u32 for calculations */
	uint8_t tmp1, tmp2;	/* Temporary u8 for register pair instructions */
	uint8_t imm8;		/* Immediate byte at PC */
	uint16_t imm16;		/* Immediate word at PC */

	c->instructions++;
	c->cycles += C_CYCLES[opcode];

	imm8 = cpu_get_byte(c, c->pc);
	imm16 = cpu_get_word(c, c->pc);
	tmp16 = 0;
	tmp32 = 0;
	tmp1 = tmp2 = 0;

	switch (opcode)
	{
		/* NOP */
		case 0x00:
		case 0x08:
		case 0x10:
		case 0x18:
		case 0x20:
		case 0x28:
		case 0x30: 
		case 0xcb:
		case 0xdd:
		case 0xed:
		case 0xfd:
		case 0x38: break;

		/* RET alternative, */
		case 0xd9: inst_ret (c, true); break;

		/* CARRY */
		case 0x37: c->flag_c = 1; break;
		case 0x2f: c->a = ~c->a; break;
		case 0x3f: c->flag_c ^= 1; break;


		case 0xc3: inst_jmp (c, true, imm16); break; /* jmp */
		case 0xda: inst_jmp (c, c->flag_c, imm16); break; /* jc */
		case 0xd2: inst_jmp (c, !c->flag_c, imm16); break; /* jnc */
		case 0xca: inst_jmp (c, c->flag_z, imm16); break; /* jz */
		case 0xc2: inst_jmp (c, !c->flag_z, imm16); break; /* jnz */
		case 0xfa: inst_jmp (c, c->flag_s, imm16); break; /* jm */
		case 0xf2: inst_jmp (c, !c->flag_s, imm16); break; /* jp */
		case 0xea: inst_jmp (c, c->flag_p, imm16); break; /* jpe */
		case 0xe2: inst_jmp (c, !c->flag_p, imm16); break; /* jpo */
		
		case 0xcd: inst_call (c, true, imm16); break; /* call */
		case 0xdc: inst_call (c, c->flag_c, imm16); break; /* cc */
		case 0xd4: inst_call (c, !c->flag_c, imm16); break; /* cnc */
		case 0xcc: inst_call (c, c->flag_z, imm16); break; /* cz */
		case 0xc4: inst_call (c, !c->flag_z, imm16); break; /* cnz */
		case 0xfc: inst_call (c, c->flag_s, imm16); break; /* cm */
		case 0xf4: inst_call (c, !c->flag_s, imm16); break; /* cp */
		case 0xec: inst_call (c, c->flag_p, imm16); break; /* cpe */
		case 0xe4: inst_call (c, !c->flag_p, imm16); break; /* cpo */
		
		case 0xc9: inst_ret (c, true); break; /* ret */
		case 0xd8: inst_ret (c, c->flag_c); break; /* rc */
		case 0xd0: inst_ret (c, !c->flag_c); break; /* rnc */
		case 0xc8: inst_ret (c, c->flag_z); break; /* rz */
		case 0xc0: inst_ret (c, !c->flag_z); break; /* rnz */
		case 0xf8: inst_ret (c, c->flag_s); break; /* rm */
		case 0xf0: inst_ret (c, !c->flag_s); break; /* rp */
		case 0xe8: inst_ret (c, c->flag_p); break; /* rpe */
		case 0xe0: inst_ret (c, !c->flag_p); break; /* rpo */

		case 0x01: cpu_set_bc (c, imm16); c->pc+=2; break; /* LXI B */
		case 0x11: cpu_set_de (c, imm16); c->pc+=2; break; /* LXI D */
		case 0x21: cpu_set_hl (c, imm16); c->pc+=2; break; /* LXI H */
		case 0x31: c->sp = imm16; c->pc+=2; break; /* LXI SP */

		case 0x06: c->b = imm8; c->pc++; break; /* MVI B */
		case 0x0e: c->c = imm8; c->pc++; break; /* MVI C */
		case 0x16: c->d = imm8;	c->pc++; break; /* MVI D */
		case 0x1e: c->e = imm8; c->pc++; break; /* MVI E */
		case 0x26: c->h = imm8;	c->pc++; break; /* MVI H */
		case 0x2e: c->l = imm8; c->pc++; break; /* MVI L */
		case 0x36: cpu_set_byte (c, cpu_get_hl (c), imm8); c->pc++; break; /* MVI M */
		case 0x3e: c->a = imm8; c->pc++; break; /* MVI A */

		case 0xc6: 
			/* ADI - Add immediate to accumulator */
			tmp16 = c->a + imm8;
			c->a = tmp16 & 0xff;
			cpu_flags_set_carry_from_word(c, tmp16);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			c->pc++;
			break;
		case 0xce: 
			/* ACI - Move Immediate data */
			tmp16 = c->a + imm8 + c->flag_c;
			c->a = tmp16 & 0xff;
			cpu_flags_set_carry_from_word(c, tmp16 & 0xff);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			c->pc++;
			break;
		case 0xd6:
			/* SUI - Subtract immediate from accumulator */
			tmp16 = c->a + FLIP(imm8);
			c->a = tmp16 & 0xff;
			cpu_flags_set_carry_from_word(c, tmp16);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			c->pc++;
			break;
		case 0xde:
			/* SBI - Subtract immediate from accumulator with borrow */
			tmp16 = c->a + FLIP(imm8) + FLIP(c->flag_c);
			c->a = tmp16 & 0xff;
			cpu_flags_set_carry_from_word(c, tmp16);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			c->pc++;
			break;
		case 0xe6: 
			/* ANI - And immediate with accumulator */
			tmp16 = c->a & imm8;
			c->a = tmp16 & 0xff;
			cpu_flags_set_carry_from_word(c, tmp16);
			cpu_flags_set_zsp(c, c->a);
			c->pc++;
			break;
		case 0xee:
			/* XRI - Xor immediate with accumulator */
			tmp16 = c->a ^ imm8;
			c->a = tmp16 & 0xff;
			cpu_flags_set_carry_from_word(c, tmp16);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			c->pc++;
			break;
		case 0xf6:
			/* ORI - Or immediate with accumulator */
			tmp16 = c->a | imm8;
			c->a = tmp16 & 0xff;
			cpu_flags_set_carry_from_word(c, tmp16);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			c->pc++;
			break;
		case 0xfe:
			/* CPI - Compare immediate with accumulator */
			/* A is not changed by this operation, only the FLAGS */
			tmp16 = c->a + FLIP(imm8);
			cpu_flags_set_carry_from_word(c, tmp16);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			c->pc++;
			break;

		/* DATA TRANSFER */
		case 0x0a: c->a = cpu_deref_bc (c); break; /* LDAX B */
		case 0x1a: c->a = cpu_deref_de (c); break; /* LDAX D */
		case 0x40: c->b = c->b; break; /* MOV B, B */
		case 0x41: c->b = c->c; break; /* MOV B, C */
		case 0x42: c->b = c->d; break; /* MOV B, D */
		case 0x43: c->b = c->e; break; /* MOV B, E */
		case 0x44: c->b = c->h; break; /* MOV B, H */
		case 0x45: c->b = c->l; break; /* MOV B, L */
		case 0x46: c->b = cpu_deref_hl (c); break; /* MOV B, M */
		case 0x47: c->b = c->a; break; /* MOV B, A */
		case 0x48: c->c = c->b; break; /* MOV C, B */
		case 0x49: c->c = c->c; break; /* MOV C, C */
		case 0x4a: c->c = c->d; break; /* MOV C, D */
		case 0x4b: c->c = c->e; break; /* MOV C, E */
		case 0x4c: c->c = c->h; break; /* MOV C, H */
		case 0x4d: c->c = c->l; break; /* MOV C, L */
		case 0x4e: c->c = cpu_deref_hl (c); break; /* MOV C, M */
		case 0x4f: c->c = c->a; break; /* MOV C, A */
		case 0x50: c->d = c->b; break; /* MOV D, B */
		case 0x51: c->d = c->c; break; /* MOV D, C */
		case 0x52: c->d = c->d; break; /* MOV D, D */
		case 0x53: c->d = c->e; break; /* MOV D, E */
		case 0x54: c->d = c->h; break; /* MOV D, H */
		case 0x55: c->d = c->l; break; /* MOV D, L */
		case 0x56: c->d = cpu_deref_hl (c); break; /* MOV D, M */
		case 0x57: c->d = c->a; break; /* MOV D, A */
		case 0x58: c->e = c->b; break; /* MOV E, B */
		case 0x59: c->e = c->c; break; /* MOV E, C */
		case 0x5a: c->e = c->d; break; /* MOV E, D */
		case 0x5b: c->e = c->e; break; /* MOV E, E */
		case 0x5c: c->e = c->h; break; /* MOV E, H */
		case 0x5d: c->e = c->l; break; /* MOV E, L */
		case 0x5e: c->e = cpu_deref_hl (c); break; /* MOV E, M */
		case 0x5f: c->e = c->a; break; /* MOV E, A */
		case 0x60: c->h = c->b; break; /* MOV H, B */
		case 0x61: c->h = c->c; break; /* MOV H, C */
		case 0x62: c->h = c->d; break; /* MOV H, D */
		case 0x63: c->h = c->e; break; /* MOV H, E */
		case 0x64: c->h = c->h; break; /* MOV H, H */
		case 0x65: c->h = c->l; break; /* MOV H, L */
		case 0x66: c->h = cpu_deref_hl (c); break; /* MOV H, M */
		case 0x67: c->h = c->a; break; /* MOV H, A */
		case 0x68: c->l = c->b; break; /* MOV L, B */
		case 0x69: c->l = c->c; break; /* MOV L, C */
		case 0x6a: c->l = c->d; break; /* MOV L, D */
		case 0x6b: c->l = c->e; break; /* MOV L, E */
		case 0x6c: c->l = c->h; break; /* MOV L, H */
		case 0x6d: c->l = c->l; break; /* MOV L, L */
		case 0x6e: c->l = cpu_deref_hl (c); break; /* MOV L, M */
		case 0x6f: c->l = c->a; break; /* MOV L, A */
		case 0x70: cpu_set_byte (c, cpu_get_hl (c), c->b); break; /* MOV M, B */
		case 0x71: cpu_set_byte (c, cpu_get_hl (c), c->c); break; /* MOV M, C */
		case 0x72: cpu_set_byte (c, cpu_get_hl (c), c->d); break; /* MOV M, D */
		case 0x73: cpu_set_byte (c, cpu_get_hl (c), c->e); break; /* MOV M, E */
		case 0x74: cpu_set_byte (c, cpu_get_hl (c), c->h); break; /* MOV M, H */
		case 0x75: cpu_set_byte (c, cpu_get_hl (c), c->l); break; /* MOV M, L */
		case 0x77: cpu_set_byte (c, cpu_get_hl (c), c->a); break; /* MOV M, A */
		case 0x78: c->a = c->b; break; /* MOV A, B */
		case 0x79: c->a = c->c; break; /* MOV A, C */
		case 0x7a: c->a = c->d; break; /* MOV A, D */
		case 0x7b: c->a = c->e; break; /* MOV A, E */
		case 0x7c: c->a = c->h; break; /* MOV A, H */
		case 0x7d: c->a = c->l; break; /* MOV A, L */
		case 0x7e: c->a = cpu_deref_hl (c); break; /* MOV A, M */
		case 0x7f: c->a = c->a; break; /* MOV A, A */

		case 0x02: cpu_set_byte (c, cpu_get_bc (c), c->a); break; /* STAX B */
		case 0x12: cpu_set_byte (c, cpu_get_de (c), c->a); break; /* STAX D */

		case 0xc5: cpu_stack_push (c, cpu_get_bc (c)); break; /* PUSH B */
		case 0xd5: cpu_stack_push (c, cpu_get_de (c)); break; /* PUSH D */
		case 0xe5: cpu_stack_push (c, cpu_get_hl (c)); break; /* PUSH H */
		case 0xf5: cpu_stack_push_psw (c); break; /* PUSH PSW */
		case 0xc1: cpu_set_bc (c, cpu_stack_pop (c)); break; /* POP B */
		case 0xd1: cpu_set_de (c, cpu_stack_pop (c)); break; /* POP D */
		case 0xe1: cpu_set_hl (c, cpu_stack_pop (c)); break; /* POP H */
		case 0xf1: cpu_stack_pop_psw (c); break; /* POP PSW */

		case 0x09:
			/* DAD B */
			tmp32 = cpu_get_bc(c) + cpu_get_hl(c);
			cpu_flags_set_carry_from_word(c, tmp32 & 0xff);
			cpu_set_hl(c, tmp32 & 0xff);
			break;
		case 0x19:
			/* DAD D */
			tmp32 = cpu_get_de(c) + cpu_get_hl(c);
			cpu_flags_set_carry_from_word(c, tmp32 & 0xffff);
			cpu_set_hl(c, tmp32 & 0xffff);
			break;
		case 0x29: 
			/* DAD H */
			tmp32 = cpu_get_hl(c) + cpu_get_hl(c);
			cpu_flags_set_carry_from_word(c, tmp32 & 0xffff);
			cpu_set_hl(c, tmp32 & 0xffff);
			break;

		case 0x39:
			/* DAD SP */
			tmp32 = c->sp + cpu_get_hl(c);
			cpu_flags_set_carry_from_word(c, tmp32);
			cpu_set_hl(c, tmp32);
			break;
		case 0x03:
			/* INX B */
			cpu_set_bc(c, cpu_get_bc(c) + 1);
			break;
		case 0x13: 
			/* INX D */
			cpu_set_de(c, cpu_get_de(c) + 1);
			break;
		case 0x23:
			/* INX H */
			cpu_set_hl(c, cpu_get_hl(c) + 1);
			break;
		case 0x33:
			/* INX SP */
			c->sp++;
			break;
		case 0x0b: 
			/* DCX B */
			cpu_set_bc(c, cpu_get_bc(c) - 1);
			break;
		case 0x1b:
			/* DCX D */
			cpu_set_de(c, cpu_get_de(c) - 1);
			break;
		case 0x2b:
			/* DCX H */
			cpu_set_hl(c, cpu_get_hl(c) - 1);
			break;
		case 0x3b:
			/* DCX SP */
			c->sp--;
			break;
		case 0xeb: 
			/* XCHG */
			tmp1 = c->h;
			c->h = c->d;
			c->d = tmp1;
			tmp1 = c->l;
			c->l = c->e;
			c->e = tmp1;
			break;
		case 0xe3:
			/* XTHL */
			tmp1 = c->l;
			tmp2 = c->h;

			c->l = cpu_deref_sp(c, 0);
			c->h = cpu_deref_sp(c, 1);

			cpu_set_byte(c, c->sp, tmp1); /* l */
			cpu_set_byte(c, c->sp + 1, tmp2); /* h */
			break;
		case 0xf9:
			/* SPHL */
			c->sp = cpu_get_hl(c);
			break;

		/* SINGLE REGISTER */
		case 0x3c: inr (c, REG (a)); break;
		case 0x04: inr (c, REG (b)); break;
		case 0x0c: inr (c, REG (c)); break;
		case 0x14: inr (c, REG (d)); break;
		case 0x1c: inr (c, REG (e)); break;
		case 0x24: inr (c, REG (h)); break;
		case 0x2c: inr (c, REG (l)); break;
		case 0x34:
			/* INR M */
			tmp16 = cpu_deref_hl(c) + 1;
			cpu_set_byte(c, cpu_get_hl(c), tmp16 & 0xff);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			cpu_flags_set_carry_from_word(c, tmp16);
			break;
		case 0x3d: dcr (c, REG (a)); break;
		case 0x05: dcr (c, REG (b)); break;
		case 0x0d: dcr (c, REG (c)); break;
		case 0x15: dcr (c, REG (d)); break;
		case 0x1d: dcr (c, REG (e)); break;
		case 0x25: dcr (c, REG (h)); break;
		case 0x2d: dcr (c, REG (l)); break;
		case 0x35:
			/* DCR M */
			tmp16 = cpu_deref_hl(c) + FLIP(1);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			cpu_flags_set_carry_from_word(c, tmp16);
			cpu_set_byte(c, cpu_get_hl(c), tmp16 & 0xff);
			break;
		case 0x27:
			/* DAA */
			tmp16 = 0;
			if ((c->a & 0x0f) > 9 || c->flag_ac) {
				tmp16 += 6;
			}

			if (((c->a >> 4) > 9) || (c->flag_c)) {
				tmp16 += 0x60;
				c->flag_c = 1;
				tmp16 += c->a;
				cpu_flags_set_zsp(c, tmp16 & 0xff);
				cpu_flags_set_carry_from_word(c, tmp16);
			}
			break;

		/* ROTATE ACCUMULATOR */
		case 0x07:
			/* RLC - Rotate left */
			tmp1 = (c->a & 0x80) >> 7; /* bit 7 */
			c->a <<= 1;
			c->a |= tmp1;
			c->flag_c = tmp1;
			break;
		case 0x0f:
			/* RRC - Rotate right */
			tmp1 = (c->a & 0x1); /* bit 0 */
			c->a >>= 1;
			c->a |= tmp1 << 7;
			c->flag_c = (tmp1 != 0);
			break;
		case 0x1f:
			/* RAR - Rotate right through carry */ 
			tmp1 = c->a & 0x1; /* bit 0 */
			c->a = c->a >> 1;
			c->a = (c->a | (c->flag_c << 7));
			c->flag_c = tmp1;
			break;
		case 0x17:
			/* RAL - Rotate left through carry*/
			tmp1 = (c->a & 0x80) >> 7; /* bit 7 */
			c->a <<= 1;
			c->a |= c->flag_c;
			c->flag_c = tmp1;
			break;

		/* I/O */
		case 0xdb:
			/* IN */
			tmp1 = imm8; /* device number */
			c->pc++;

			debug_device(tmp1);

			if (tmp1 == DEVICE_INP0) {
				c->a = c->i0;
			} else if (tmp1 == DEVICE_INP1) {
				c->a = c->i1;
			} else if (tmp1 == DEVICE_INP2) {
				c->a = c->i2;
			} else if (tmp1 == DEVICE_SHIFT_IN) {
				c->a = (c->shift >> ((8 - c->shift_amt)) & 0xff);
			}
			break;
		case 0xd3:
			/* OUT */
			tmp1 = imm8;
			c->pc++;

			debug_device(tmp1);

			if (tmp1 == DEVICE_SHIFT_AMT) {
				c->shift_amt = (c->a & 7);
			} else if (tmp1 == DEVICE_SOUND1) {
				c->o3 = c->a;
			} else if (tmp1 == DEVICE_SHIFT_DATA) {
				c->shift >>= 8;
				c->shift |= (c->a << 8);
			} else if (tmp1 == DEVICE_SOUND2) {
				c->o5 = c->a;
			} else if (tmp1 == DEVICE_WATCHDOG) {
				c->o6 = c->a;
			}
			break;
		case 0x76: 
			/* HALT */
			debug_msg("CPU Halted");
			c->halt = 1;
			break;
		/* REGISTER OR MEMORY TO ACCUMULATOR */
		case 0x87: add (c, REG (a)); break;
		case 0x80: add (c, REG (b)); break;
		case 0x81: add (c, REG (c)); break;
		case 0x82: add (c, REG (d)); break;
		case 0x83: add (c, REG (e)); break;
		case 0x84: add (c, REG (h)); break;
		case 0x85: add (c, REG (l)); break;
		case 0x86:
			/* ADD M */
			tmp16 = c->a + cpu_deref_hl(c);
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			cpu_flags_set_carry_from_word(c, tmp16);
			c->a = tmp16 & 0xff;
			break;
		case 0x8f: adc (c, REG (a)); break;
		case 0x88: adc (c, REG (b)); break;
		case 0x89: adc (c, REG (c)); break;
		case 0x8a: adc (c, REG (d)); break;
		case 0x8b: adc (c, REG (e)); break;
		case 0x8c: adc (c, REG (h)); break;
		case 0x8d: adc (c, REG (l)); break;
		case 0x8e: 
			/* ADC M */
			tmp16 = c->a + cpu_deref_hl(c) + c->flag_c;
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			cpu_flags_set_carry_from_word(c, tmp16);
			c->a = tmp16 & 0xff;
			break;
		case 0x97: sub (c, REG (a)); break;
		case 0x90: sub (c, REG (b)); break;
		case 0x91: sub (c, REG (c)); break;
		case 0x92: sub (c, REG (d)); break;
		case 0x93: sub (c, REG (e)); break;
		case 0x94: sub (c, REG (h)); break;
		case 0x95: sub (c, REG (l)); break;
		case 0x96: sub_m (c); break;
		case 0x9f: sbb (c, REG (a)); break;
		case 0x98: sbb (c, REG (b)); break;
		case 0x99: sbb (c, REG (c)); break;
		case 0x9a: sbb (c, REG (d)); break;
		case 0x9b: sbb (c, REG (e)); break;
		case 0x9c: sbb (c, REG (h)); break;
		case 0x9d: sbb (c, REG (l)); break;
		case 0x9e: sbb_m (c); break;
		case 0xa7: ana (c, REG (a)); break;
		case 0xa0: ana (c, REG (b)); break;
		case 0xa1: ana (c, REG (c)); break;
		case 0xa2: ana (c, REG (d)); break;
		case 0xa3: ana (c, REG (e)); break;
		case 0xa4: ana (c, REG (h)); break;
		case 0xa5: ana (c, REG (l)); break;
		case 0xa6: ana_m (c); break;
		case 0xaf: xra (c, REG (a)); break;
		case 0xa8: xra (c, REG (b)); break;
		case 0xa9: xra (c, REG (c)); break;
		case 0xaa: xra (c, REG (d)); break;
		case 0xab: xra (c, REG (e)); break;
		case 0xac: xra (c, REG (h)); break;
		case 0xad: xra (c, REG (l)); break;
		case 0xae:
			/* XRA M */
			tmp16 = c->a ^ (cpu_deref_hl(c));
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			cpu_flags_set_carry_from_word(c, tmp16);
			c->a = tmp16 & 0xff;
			break;
		case 0xb7: ora (c, REG (a)); break;
		case 0xb0: ora (c, REG (b)); break;
		case 0xb1: ora (c, REG (c)); break;
		case 0xb2: ora (c, REG (d)); break;
		case 0xb3: ora (c, REG (e)); break;
		case 0xb4: ora (c, REG (h)); break;
		case 0xb5: ora (c, REG (l)); break;
		case 0xb6: 
			/* ORA M */
			tmp16 = c->a | (cpu_deref_hl(c));
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			cpu_flags_set_carry_from_word(c, tmp16);
			c->a = tmp16 & 0xff;
			break;
		case 0xb8: cmp (c, REG (b)); break;
		case 0xb9: cmp (c, REG (c)); break;
		case 0xba: cmp (c, REG (d)); break;
		case 0xbb: cmp (c, REG (e)); break;
		case 0xbc: cmp (c, REG (h)); break;
		case 0xbd: cmp (c, REG (l)); break;
		case 0xbe: 
			/* CMP M */
			tmp16 = c->a + FLIP(cpu_deref_hl(c));
			cpu_flags_set_zsp(c, tmp16 & 0xff);
			cpu_flags_set_carry_from_word(c, tmp16);
			break;
		case 0xbf: cmp (c, REG (a)); break;

		case 0x32: cpu_set_byte (c, imm16, c->a); c->pc+=2; break; /* STA adr */
		case 0x3a: c->a = cpu_get_byte (c, imm16); c->pc+=2; break; /* LDA adr */
		case 0x22: cpu_set_word (c, imm16, cpu_get_hl (c)); c->pc+=2; break; /* SHLD adr */
		case 0x2a: cpu_set_hl (c, cpu_get_word (c, imm16)); c->pc+=2; break; /* LHLD adr */
		case 0xe9: c->pc = cpu_get_hl (c); break; /* PCHL */

		/* INTERRUPTS */
		case 0xfb: c->interrupts_enabled = true; break;
		case 0xf3: c->interrupts_enabled = false; break;
		case 0xc7: inst_call (c, true, 0x0); break;
		case 0xcf: inst_call (c, true, 0x8); break;
		case 0xd7: inst_call (c, true, 0x10); break;
		case 0xdf: inst_call (c, true, 0x18); break;
		case 0xe7: inst_call (c, true, 0x20); break;
		case 0xef: inst_call (c, true, 0x28); break;
		case 0xf7: inst_call (c, true, 0x30); break;
		case 0xff: inst_call (c, true, 0x38); break;

		default: cpu_unimplemented (c); break;
	}
}
