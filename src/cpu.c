#include "cpu.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>

const uint8_t CYCLES[] = {
	4, 10, 7, 5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5, 7,	 4,	 4,	 10, 16, 5,	 5,
	5, 7,  4, 4,  10, 16, 5,  5,  5,  7,  4,  4,  10, 13, 5,  10, 10, 10, 4,  4,  10, 13, 5,  5,  5,  7,  4,  5,  5,  5, 5,	 5,	 5,	 7,	 5,	 5,	 5,
	5, 5,  5, 5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, 5,	 5,	 5,	 5,	 5,	 5,	 7,
	5, 7,  7, 7,  7,  7,  7,  7,  7,  5,  5,  5,  5,  5,  5,  7,  5,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4, 4,	 7,	 4,	 4,	 4,	 4,	 4,
	4, 4,  7, 4,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4, 4,	 4,	 4,	 4,	 7,	 4,	 4,
	4, 4,  4, 4,  4,  7,  4,  5,  10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 11, 7, 11, 5,	 10, 10, 10, 11, 11,
	7, 11, 5, 10, 10, 18, 11, 11, 7,  11, 5,  5,  10, 5,  11, 11, 7,  11, 5,  10, 10, 4,  11, 11, 7,  11, 5,  5,  10, 4, 11, 11, 7,	 11,
};

const char *INSTRUCTIONS[] = {
	"NOP",	   "LXI B",	  "STAX B",	 "INX B",	"INR B",   "DCR B",	  "MVI B",	 "RLC",		"NOP",	   "DAD B",	  "LDAX B",	 "DCX B",	 "INR C",
	"DCR C",   "MVI C",	  "RRC",	 "NOP",		"LXI D",   "STAX D",  "INX D",	 "INR D",	"DCR D",   "MVI D",	  "RALC",	 "NOP",		 "DAD D",
	"LDAX D",  "DCX D",	  "INR E",	 "DCR E",	"MVI E",   "RAR",	  "NOP",	 "LXI H",	"SHLD",	   "INX H",	  "INR H",	 "DCR H",	 "MVI H",
	"DAA",	   "NOP",	  "DAD H",	 "LHLD",	"DCX H",   "INR L",	  "DCR L",	 "MVI L",	"CMA",	   "NOP",	  "LXI SP",	 "STA",		 "INX SP",
	"INR M",   "DCR M",	  "MVI M",	 "STC",		"NOP",	   "DAD SP",  "LDA",	 "DCX SP",	"INR A",   "DCR A",	  "MVI A",	 "CMC",		 "MOV B,B",
	"MOV B,C", "MOV B,D", "MOV B,E", "MOV B,H", "MOV B,L", "MOV B,M", "MOV B,A", "MOV C,B", "MOV C,C", "MOV C,D", "MOV C,E", "MOV C,H",	 "MOV C,L",
	"MOV C,M", "MOV C,A", "MOV D,B", "MOV D,C", "MOV D,D", "MOV D,E", "MOV D,H", "MOV D,L", "MOV D,M", "MOV D,A", "MOV E,B", "MOV E,C",	 "MOV E,D",
	"MOV E,E", "MOV E,H", "MOV E,L", "MOV E,M", "MOV E,A", "MOV H,B", "MOV H,C", "MOV H,D", "MOV H,E", "MOV H,H", "MOV H,L", "MOV H,M",	 "MOV H,A",
	"MOV L,B", "MOV L,C", "MOV L,D", "MOV L,E", "MOV L,H", "MOV L,L", "MOV L,M", "MOV L,A", "MOV M,B", "MOV M,C", "MOV M,D", "MOV M,E",	 "MOV M,H",
	"MOV M,L", "HLT",	  "MOV M,A", "MOV A,B", "MOV A,C", "MOV A,D", "MOV A,E", "MOV A,H", "MOV A,L", "MOV A,M", "MOV A,A", "ADD B",	 "ADD C",
	"ADD D",   "ADD E",	  "ADD H",	 "ADD L",	"ADD M",   "ADD A",	  "ADC B",	 "ADC C",	"ADC D",   "ADC E",	  "ADC H",	 "ADC L",	 "ADC M",
	"ADC A",   "SUB B",	  "SUB C",	 "SUB D",	"SUB E",   "SUB H",	  "SUB L",	 "SUB M",	"SUB A",   "SBB B",	  "SBB C",	 "SBB D",	 "SBB E",
	"SBB H",   "SBB L",	  "SBB M",	 "SBB A",	"ANA B",   "ANA C",	  "ANA D",	 "ANA E",	"ANA H",   "ANA L",	  "ANA M",	 "ANA A",	 "XRA B",
	"XRA C",   "XRA D",	  "XRA E",	 "XRA H",	"XRA L",   "XRA M",	  "XRA A",	 "ORA B",	"ORA C",   "ORA D",	  "ORA E",	 "ORA H",	 "ORA L",
	"ORA M",   "ORA A",	  "CMP B",	 "CMP C",	"CMP D",   "CMP E",	  "CMP H",	 "CMP L",	"CMP M",   "CMP A",	  "RNZ",	 "POP B",	 "JNZ",
	"JMP",	   "CNZ",	  "PUSH B",	 "ADI",		"RST 0",   "RZ",	  "RET",	 "JZ",		"NOP",	   "CZ",	  "CALL",	 "ACI",		 "RST 1",
	"RNC",	   "POP D",	  "JNC",	 "OUT",		"CNC",	   "PUSH D",  "SUI",	 "RST 2",	"RC",	   "NOP",	  "JC",		 "IN",		 "CC",
	"NOP",	   "SBI",	  "RST 3",	 "RPO",		"POP H",   "JPO",	  "XTHL",	 "CPO",		"PUSH H",  "ANI",	  "RST 4",	 "RPE",		 "PCHL",
	"JPE",	   "XCHG",	  "CPE",	 "NOP",		"XRI",	   "RST 5",	  "RP",		 "POP PSW", "JP",	   "DI",	  "CP",		 "PUSH PSW", "ORI",
	"RST 6",   "RM",	  "SPHL",	 "JM",		"EI",	   "CM",	  "NOP",	 "CPI",		"RST 7"};

// Core
void cpu_init (cpu *c)
{
	c->a = 0;
	c->b = 0;
	c->c = 0;
	c->d = 0;
	c->e = 0;
	c->h = 0;
	c->l = 0;

	c->flag_z  = 0;
	c->flag_s  = 0;
	c->flag_p  = 0;
	c->flag_c  = 0;
	c->flag_ac = 0;

	c->pc = 0;
	c->sp = 0;

	c->cycles		= 0;
	c->instructions = 0;

	c->memory			  = NULL;
	c->halt				  = 0;
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

	c->shift	 = 0;
	c->shift_amt = 0;
}
void cpu_set_memory (cpu *c, uint8_t *memory_ptr) { c->memory = memory_ptr; }
void cpu_disasm (cpu *c)
{
	printf ("0x%04x\t%02x\t\t%02x|%02x|%02x|%02x|%02x|%02x|%02x|%04x|%04x|%d%d%d%d%d\t%s", c->pc, cpu_get_byte (c, c->pc), c->a, c->b, c->c, c->d,
			c->e, c->h, c->l, c->sp, c->pc, c->flag_z, c->flag_s, c->flag_p, c->flag_c, c->flag_ac, INSTRUCTIONS[cpu_get_byte (c, c->pc)]);
}

// Memory
uint8_t	 cpu_get_byte (cpu *c, uint16_t address) { return c->memory[address]; }
void	 cpu_set_byte (cpu *c, uint16_t address, uint8_t val) { c->memory[address] = val; }
uint16_t cpu_get_word (cpu *c, uint16_t address) { return c->memory[address + 1] << 8 | c->memory[address]; }

void cpu_set_word (cpu *c, uint16_t address, uint16_t val)
{
	cpu_set_byte (c, address, val & 0xff);
	cpu_set_byte (c, address + 1, val >> 8);
}

// Register pairs
uint16_t cpu_get_bc (cpu *c) { return c->b << 8 | c->c; }
uint16_t cpu_get_de (cpu *c) { return c->d << 8 | c->e; }
uint16_t cpu_get_hl (cpu *c) { return c->h << 8 | c->l; }

uint16_t cpu_get_psw (cpu *c)
{
	uint16_t ret = 0;
	ret |= c->a << 8;
	ret |= cpu_get_flags (c) & 0xff;
	return ret;
}

void cpu_set_bc (cpu *c, uint16_t val)
{
	c->b = val >> 8;
	c->c = val & 0xff;
}

void cpu_set_de (cpu *c, uint16_t val)
{
	c->d = val >> 8;
	c->e = val & 0xff;
}

void cpu_set_hl (cpu *c, uint16_t val)
{
	c->h = val >> 8;
	c->l = val & 0xff;
}

uint16_t cpu_deref_bc (cpu *c) { return cpu_get_byte (c, cpu_get_bc (c)); }
uint16_t cpu_deref_de (cpu *c) { return cpu_get_byte (c, cpu_get_de (c)); }
uint16_t cpu_deref_hl (cpu *c) { return cpu_get_byte (c, cpu_get_hl (c)); }
uint16_t cpu_deref_sp (cpu *c, uint16_t offset) { return cpu_get_byte (c, c->sp + offset); }

// Stack
void stack_push (cpu *c, uint16_t val)
{
	c->sp -= 2;
	cpu_set_word (c, c->sp, val);
}

uint16_t stack_pop (cpu *c)
{
	uint16_t ret = cpu_get_word (c, c->sp);
	c->sp += 2;
	return ret;
}

void stack_push_psw (cpu *c) { stack_push (c, cpu_get_psw (c)); }

void stack_pop_psw (cpu *c)
{
	uint16_t psw = stack_pop (c);
	c->a		 = psw >> 8;
	c->flag_s	 = psw >> 7 & 0x1;
	c->flag_z	 = psw >> 6 & 0x1;
	c->flag_ac	 = psw >> 4 & 0x1;
	c->flag_p	 = psw >> 2 & 0x1;
	c->flag_c	 = psw & 0x1;
}

// Flags - private calculation operations
uint8_t flags_calc_parity (uint8_t n)
{
	uint8_t parity = 0;
	while (n)
	{
		parity ^= (n & 1);
		n >>= 1;
	}
	return parity;
}

uint8_t flags_calc_zero (uint8_t n) { return n == 0; }

uint8_t flags_calc_sign (uint8_t n) { return (n & 0x80) == 0x80; }

uint8_t flags_calc_carry (uint8_t f, uint8_t g, uint8_t modulator, uint8_t carry_bit)
{
	uint16_t raw  = f + g + modulator;
	uint16_t xord = raw ^ f ^ g;
	return (xord & (1 << carry_bit)) != 0;
}

uint8_t flags_calc_carry_and (uint8_t f, uint8_t g, uint8_t carry_bit)
{
	uint16_t raw  = f & g;
	uint16_t xord = raw ^ f ^ g;
	return (xord & (1 << carry_bit)) != 0;
}

uint8_t flags_calc_carry_or (uint8_t f, uint8_t g, uint8_t carry_bit)
{
	uint16_t raw  = f | g;
	uint16_t xord = raw ^ f ^ g;
	return (xord & (1 << carry_bit)) != 0;
}

uint8_t flags_calc_carry_xor (uint8_t f, uint8_t g, uint8_t carry_bit)
{
	uint16_t raw  = f ^ g;
	uint16_t xord = raw ^ f ^ g;
	return (xord & (1 << carry_bit)) != 0;
}

// Flags
uint8_t cpu_get_flags (cpu *c)
{
	uint8_t ret = c->flag_s << 7;
	ret |= c->flag_z << 6;
	ret |= 0 << 5;
	ret |= c->flag_ac << 4;
	ret |= 0 << 3;
	ret |= c->flag_p << 2;
	ret |= 1 << 1;
	ret |= c->flag_c;
	return ret;
}

void cpu_set_flags_zsp (cpu *c, uint8_t val)
{
	c->flag_z = flags_calc_zero (val);
	c->flag_s = flags_calc_sign (val);
	c->flag_p = flags_calc_parity (val);
}
void cpu_set_flags_c (cpu *c, uint8_t f, uint8_t g, uint8_t modulator) { c->flag_c = flags_calc_carry (f, g, modulator, 8); }
void cpu_set_flags_c_and (cpu *c, uint8_t f, uint8_t g) { c->flag_c = flags_calc_carry_and (f, g, 8); }
void cpu_set_flags_c_or (cpu *c, uint8_t f, uint8_t g) { c->flag_c = flags_calc_carry_or (f, g, 8); }
void cpu_set_flags_c_xor (cpu *c, uint8_t f, uint8_t g) { c->flag_c = flags_calc_carry_xor (f, g, 8); }
void cpu_set_flags_ac (cpu *c, uint8_t f, uint8_t g, uint8_t modulator) { c->flag_c = flags_calc_carry (f, g, modulator, 4); }
void cpu_set_flags_ac_and (cpu *c, uint8_t f, uint8_t g) { c->flag_c = flags_calc_carry_and (f, g, 4); }
void cpu_set_flags_ac_or (cpu *c, uint8_t f, uint8_t g) { c->flag_c = flags_calc_carry_or (f, g, 4); }
void cpu_set_flags_ac_xor (cpu *c, uint8_t f, uint8_t g) { c->flag_c = flags_calc_carry_xor (f, g, 4); }
void cpu_set_flags_all (cpu *c, uint8_t f, uint8_t g, uint8_t modulator)
{
	cpu_set_flags_zsp (c, f);
	cpu_set_flags_c (c, f, g, modulator);
	cpu_set_flags_ac (c, f, g, modulator);
}

// Emualtion
void cpu_unimplemented (cpu *c)
{
	printf ("\nUNIMPLEMENTED INSTRUCTION %02x\n", cpu_get_byte (c, c->pc));
	printf ("Instructions executed: %lu\n", c->instructions);
	printf ("Cycles: %lu\n", c->cycles);
	exit (1);
}

void cpu_emulate (cpu *c, uint8_t opcode)
{
	/* READ HEADER FILES FOR INSTRUCTION DOCUMENTATION */
	c->instructions++;
	c->cycles += CYCLES[opcode];

	switch (opcode)
	{
		// NOP
		case 0x00:
		case 0x08:
		case 0x10:
		case 0x18:
		case 0x20:
		case 0x28:
		case 0x30:
		case 0x38:
		case 0xcb:
		case 0xd9:
		case 0xdd:
		case 0xed:
		case 0xfd: PC1; break;

		// JUMP
		case 0xe9: pchl (c); break;
		case 0xc3: jmp (c); break;
		case 0xda: jc (c); break;
		case 0xd2: jnc (c); break;
		case 0xca: jz (c); break;
		case 0xc2: jnz (c); break;
		case 0xfa: jm (c); break;
		case 0xf2: jp (c); break;
		case 0xea: jpe (c); break;
		case 0xe2: jpo (c); break;

		// CALL
		case 0xcd: call (c); break;
		case 0xdc: cc (c); break;
		case 0xd4: cnc (c); break;
		case 0xcc: cz (c); break;
		case 0xc4: cnz (c); break;
		case 0xfc: cm (c); break;
		case 0xf4: cp (c); break;
		case 0xec: cpe (c); break;
		case 0xe4: cpo (c); break;

		// RET
		case 0xc9: ret (c); break;
		case 0xd8: rc (c); break;
		case 0xd0: rnc (c); break;
		case 0xc8: rz (c); break;
		case 0xc0: rnz (c); break;
		case 0xf8: rm (c); break;
		case 0xf0: rp (c); break;
		case 0xe8: rpe (c); break;
		case 0xe0: rpo (c); break;

		// IMMEDIATE INSTRUCTIONS
		case 0x01: lxi_b (c); break;
		case 0x11: lxi_d (c); break;
		case 0x21: lxi_h (c); break;
		case 0x31: lxi_sp (c); break;

		case 0x3e: mvi (c, REG (a)); break;
		case 0x06: mvi (c, REG (b)); break;
		case 0x0e: mvi (c, REG (c)); break;
		case 0x16: mvi (c, REG (d)); break;
		case 0x1e: mvi (c, REG (e)); break;
		case 0x26: mvi (c, REG (h)); break;
		case 0x2e: mvi (c, REG (l)); break;
		case 0x36: mvi_m (c); break;

		case 0xc6: adi (c); break;
		case 0xce: aci (c); break;
		case 0xd6: sui (c); break;
		case 0xde: sbi (c); break;
		case 0xe6: ani (c); break;
		case 0xee: xri (c); break;
		case 0xf6: ori (c); break;
		case 0xfe: cpi (c); break;

		// DATA TRANSFER
		case 0x0a: ldax_b (c); break;
		case 0x1a: ldax_d (c); break;
		case 0x77: mov_m (c, REG (a)); break;
		case 0x70: mov_m (c, REG (b)); break;
		case 0x71: mov_m (c, REG (c)); break;
		case 0x72: mov_m (c, REG (d)); break;
		case 0x73: mov_m (c, REG (e)); break;
		case 0x74: mov_m (c, REG (h)); break;
		case 0x75: mov_m (c, REG (l)); break;
		case 0x47: mov (c, REG (b), REG (a)); break;
		case 0x40: mov (c, REG (b), REG (b)); break;
		case 0x41: mov (c, REG (b), REG (c)); break;
		case 0x42: mov (c, REG (b), REG (d)); break;
		case 0x43: mov (c, REG (b), REG (e)); break;
		case 0x44: mov (c, REG (b), REG (h)); break;
		case 0x45: mov (c, REG (b), REG (l)); break;
		case 0x4f: mov (c, REG (c), REG (a)); break;
		case 0x48: mov (c, REG (c), REG (b)); break;
		case 0x49: mov (c, REG (c), REG (c)); break;
		case 0x4a: mov (c, REG (c), REG (d)); break;
		case 0x4b: mov (c, REG (c), REG (e)); break;
		case 0x4c: mov (c, REG (c), REG (h)); break;
		case 0x4d: mov (c, REG (c), REG (l)); break;
		case 0x57: mov (c, REG (d), REG (a)); break;
		case 0x50: mov (c, REG (d), REG (b)); break;
		case 0x51: mov (c, REG (d), REG (c)); break;
		case 0x52: mov (c, REG (d), REG (d)); break;
		case 0x53: mov (c, REG (d), REG (e)); break;
		case 0x54: mov (c, REG (d), REG (h)); break;
		case 0x55: mov (c, REG (d), REG (l)); break;
		case 0x5f: mov (c, REG (e), REG (a)); break;
		case 0x58: mov (c, REG (e), REG (b)); break;
		case 0x59: mov (c, REG (e), REG (c)); break;
		case 0x5a: mov (c, REG (e), REG (d)); break;
		case 0x5b: mov (c, REG (e), REG (e)); break;
		case 0x5c: mov (c, REG (e), REG (h)); break;
		case 0x5d: mov (c, REG (e), REG (l)); break;
		case 0x67: mov (c, REG (h), REG (a)); break;
		case 0x60: mov (c, REG (h), REG (b)); break;
		case 0x61: mov (c, REG (h), REG (c)); break;
		case 0x62: mov (c, REG (h), REG (d)); break;
		case 0x63: mov (c, REG (h), REG (e)); break;
		case 0x64: mov (c, REG (h), REG (h)); break;
		case 0x65: mov (c, REG (h), REG (l)); break;
		case 0x6f: mov (c, REG (l), REG (a)); break;
		case 0x68: mov (c, REG (l), REG (b)); break;
		case 0x69: mov (c, REG (l), REG (c)); break;
		case 0x6a: mov (c, REG (l), REG (d)); break;
		case 0x6b: mov (c, REG (l), REG (e)); break;
		case 0x6c: mov (c, REG (l), REG (h)); break;
		case 0x6d: mov (c, REG (l), REG (l)); break;
		case 0x7f: mov (c, REG (a), REG (a)); break;
		case 0x78: mov (c, REG (a), REG (b)); break;
		case 0x79: mov (c, REG (a), REG (c)); break;
		case 0x7a: mov (c, REG (a), REG (d)); break;
		case 0x7b: mov (c, REG (a), REG (e)); break;
		case 0x7c: mov (c, REG (a), REG (h)); break;
		case 0x7d: mov (c, REG (a), REG (l)); break;

		case 0x7e: mov_m_to_dest (c, REG (a));
		case 0x46: mov_m_to_dest (c, REG (b));
		case 0x4e: mov_m_to_dest (c, REG (c));
		case 0x56: mov_m_to_dest (c, REG (d));
		case 0x5e: mov_m_to_dest (c, REG (e));
		case 0x66: mov_m_to_dest (c, REG (h));
		case 0x6e: mov_m_to_dest (c, REG (l));

		case 0x02: stax_b (c); break;
		case 0x12: stax_d (c); break;

		// REGISTER PAIR INSTRUCTIONS
		case 0xc5: push_b (c); break;
		case 0xd5: push_d (c); break;
		case 0xe5: push_h (c); break;
		case 0xf5: push_psw (c); break;
		case 0xc1: pop_b (c); break;
		case 0xd1: pop_d (c); break;
		case 0xe1: pop_h (c); break;
		case 0xf1: pop_psw (c); break;
		case 0x09: dad_b (c); break;
		case 0x19: dad_d (c); break;
		case 0x29: dad_h (c); break;
		case 0x39: dad_sp (c); break;
		case 0x03: inx_b (c); break;
		case 0x13: inx_d (c); break;
		case 0x23: inx_h (c); break;
		case 0x33: inx_sp (c); break;
		case 0x0b: dcx_b (c); break;
		case 0x1b: dcx_d (c); break;
		case 0x2b: dcx_h (c); break;
		case 0x3b: dcx_sp (c); break;
		case 0xeb: xchg (c); break;
		case 0xe3: xthl (c); break;
		case 0xf9: sphl (c); break;

		// SINGLE REGISTER INSTRUCTIONS
		case 0x3c: inr (c, REG (a)); break;
		case 0x04: inr (c, REG (b)); break;
		case 0x0c: inr (c, REG (c)); break;
		case 0x14: inr (c, REG (d)); break;
		case 0x1c: inr (c, REG (e)); break;
		case 0x24: inr (c, REG (h)); break;
		case 0x2c: inr (c, REG (l)); break;
		case 0x34: inr_m (c); break;
		case 0x3d: dcr (c, REG (a)); break;
		case 0x05: dcr (c, REG (b)); break;
		case 0x0d: dcr (c, REG (c)); break;
		case 0x15: dcr (c, REG (d)); break;
		case 0x1d: dcr (c, REG (e)); break;
		case 0x25: dcr (c, REG (h)); break;
		case 0x2d: dcr (c, REG (l)); break;
		case 0x35: dcr_m (c); break;

		// ROTATE ACCUMULATOR
		case 0x07: rlc (c); break;
		case 0x0f: rrc (c); break;
		case 0x1f: rar (c); break;
		case 0x17: ral (c); break;

		// I/O
		case 0xdb: in (c); break;
		case 0xd3: out (c); break;
		case 0x76: hlt (c); break;

		// REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS
		case 0x87: add (c, REG (a)); break;
		case 0x80: add (c, REG (a)); break;
		case 0x81: add (c, REG (a)); break;
		case 0x82: add (c, REG (a)); break;
		case 0x83: add (c, REG (a)); break;
		case 0x84: add (c, REG (a)); break;
		case 0x85: add (c, REG (a)); break;
		case 0x86: add_m (c); break;
		case 0x8f: adc (c, REG (a)); break;
		case 0x88: adc (c, REG (a)); break;
		case 0x89: adc (c, REG (a)); break;
		case 0x8a: adc (c, REG (a)); break;
		case 0x8b: adc (c, REG (a)); break;
		case 0x8c: adc (c, REG (a)); break;
		case 0x8d: adc (c, REG (a)); break;
		case 0x8e: adc_m (c); break;
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
		case 0xae: xra_m (c); break;
		case 0xb7: ora (c, REG (a)); break;
		case 0xb0: ora (c, REG (b)); break;
		case 0xb1: ora (c, REG (c)); break;
		case 0xb2: ora (c, REG (d)); break;
		case 0xb3: ora (c, REG (e)); break;
		case 0xb4: ora (c, REG (h)); break;
		case 0xb5: ora (c, REG (l)); break;
		case 0xb6: ora_m (c); break;

		// DIRECT ADDRESSING INSTRUCTIONS
		case 0x32: sta (c); break;
		case 0x3a: lda (c); break;
		case 0x22: shld (c); break;
		case 0x2a: lhld (c); break;

		default: cpu_unimplemented (c); break;
	}
}
