#include "defs.h"

const uint8_t C_CYCLES[] = {
	4, 10, 7, 5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5, 7,	 4,	 4,	 10, 16, 5,	 5,
	5, 7,  4, 4,  10, 16, 5,  5,  5,  7,  4,  4,  10, 13, 5,  10, 10, 10, 4,  4,  10, 13, 5,  5,  5,  7,  4,  5,  5,  5, 5,	 5,	 5,	 7,	 5,	 5,	 5,
	5, 5,  5, 5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, 5,	 5,	 5,	 5,	 5,	 5,	 7,
	5, 7,  7, 7,  7,  7,  7,  7,  7,  5,  5,  5,  5,  5,  5,  7,  5,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4, 4,	 7,	 4,	 4,	 4,	 4,	 4,
	4, 4,  7, 4,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4, 4,	 4,	 4,	 4,	 7,	 4,	 4,
	4, 4,  4, 4,  4,  7,  4,  5,  10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 11, 7, 11, 5,	 10, 10, 10, 11, 11,
	7, 11, 5, 10, 10, 18, 11, 11, 7,  11, 5,  5,  10, 5,  11, 11, 7,  11, 5,  10, 10, 4,  11, 11, 7,  11, 5,  5,  10, 4, 11, 11, 7,	 11,
};

const char *C_INSTRUCTIONS[] = {
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

//
// Core
//
void C_Init (cpu *c)
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

	c->paused = 0;
}
void C_DisAsm (cpu *c)
{
	printf ("0x%04x\t%02x\t\t%02x|%02x|%02x|%02x|%02x|%02x|%02x|%04x|%d%d%d%d%d|%04x\t%s", c->pc, C_GetByte (c, c->pc), c->a, c->b, c->c, c->d, c->e,
			c->h, c->l, c->sp, c->flag_z, c->flag_s, c->flag_p, c->flag_c, c->flag_ac, c->shift, C_INSTRUCTIONS[C_GetByte (c, c->pc)]);
}

void C_GenerateInterrupt (cpu *c, uint16_t addr)
{
	if (c->interrupts_enabled)
	{
		c->interrupts_enabled = 0;
		S_Push (c, c->pc);
		c->pc = addr;
	}
}

//
// Memory
//
void	C_SetMemory (cpu *c, uint8_t *memory_ptr) { c->memory = memory_ptr; }
uint8_t C_GetByte (cpu *c, uint16_t address) { return (c->memory[address]); }
void	C_SetByte (cpu *c, uint16_t address, uint8_t val)
{
	if (address >= 0x2000 && address <= 0x4000) { c->memory[address] = val; }
}
uint16_t C_GetWord (cpu *c, uint16_t address) { return c->memory[address + 1] << 8 | c->memory[address]; }
void	 C_SetWord (cpu *c, uint16_t address, uint16_t val)
{
	C_SetByte (c, address, val & 0xff);
	C_SetByte (c, address + 1, val >> 8);
}

//
// Register pairs
//
uint16_t C_GetBC (cpu *c) { return c->b << 8 | c->c; }
uint16_t C_GetDE (cpu *c) { return c->d << 8 | c->e; }
uint16_t C_GetHL (cpu *c) { return c->h << 8 | c->l; }
uint16_t C_GetPSW (cpu *c)
{
	uint16_t ret = 0;
	ret |= c->a << 8;
	ret |= C_Flags_Get (c) & 0xff;
	return ret;
}
void C_SetBC (cpu *c, uint16_t val)
{
	c->b = val >> 8;
	c->c = val & 0xff;
}
void C_SetDE (cpu *c, uint16_t val)
{
	c->d = val >> 8;
	c->e = val & 0xff;
}
void C_SetHL (cpu *c, uint16_t val)
{
	c->h = val >> 8;
	c->l = val & 0xff;
}
uint8_t C_DerefBC (cpu *c) { return C_GetByte (c, C_GetBC (c)); }
uint8_t C_DerefDE (cpu *c) { return C_GetByte (c, C_GetDE (c)); }
uint8_t C_DerefHL (cpu *c) { return C_GetByte (c, C_GetHL (c)); }
uint8_t C_DerefSP (cpu *c, uint16_t offset) { return C_GetByte (c, c->sp + offset); }

//
// Stack
//
void S_Push (cpu *c, uint16_t word)
{
	c->sp -= 2;
	C_SetWord (c, c->sp, word);
}
uint16_t S_Pop (cpu *c)
{
	uint16_t ret = C_GetWord (c, c->sp);
	c->sp += 2;
	return ret;
}
void S_PushPSW (cpu *c) { S_Push (c, C_GetPSW (c)); }
void S_PopPSW (cpu *c)
{
	uint16_t psw = S_Pop (c);
	c->a		 = psw >> 8;
	c->flag_s	 = psw >> 7 & 0x1;
	c->flag_z	 = psw >> 6 & 0x1;
	c->flag_ac	 = psw >> 4 & 0x1;
	c->flag_p	 = psw >> 2 & 0x1;
	c->flag_c	 = psw & 0x1;
}

//
// Flags
//
uint8_t F_Parity (uint8_t n)
{
	uint8_t parity = 0;
	while (n)
	{
		parity ^= (n & 1);
		n >>= 1;
	}
	return 1 - parity;
}
uint8_t F_Zero (uint8_t n)
{
	if (n == 0) { return 1; }
	else { return 0; }
}
uint8_t F_Sign (uint8_t n)
{
	if ((n & 0x80) == 0x80) { return 1; }
	else { return 0; }
}
uint8_t F_Carry (uint8_t a, uint8_t b, uint8_t carry)
{
	uint16_t sum = a + b + carry;
	return (sum > 0xff);
}
uint8_t C_Flags_Get (cpu *c)
{
	uint8_t ret = c->flag_s << 7;
	ret |= c->flag_z << 6;
	ret |= 0 << 5;
	ret |= c->flag_ac << 4;
	ret |= 0 << 3;
	ret |= c->flag_p << 2;
	ret |= 1 << 1;
	ret |= c->flag_c & 0xff;
	return ret;
}
void C_Flags_SetZSP (cpu *c, uint8_t val)
{
	c->flag_z = F_Zero (val);
	c->flag_s = F_Sign (val);
	c->flag_p = F_Parity (val);
}
void C_Flags_SetCarryAdd (cpu *c, uint8_t a, uint8_t b, uint8_t carry) { c->flag_c = F_Carry (a, b, carry); }
void C_Flags_SetCarryFromWord (cpu *c, uint16_t num) { c->flag_c = (num > 0xff); }

//
// Emulation
//
void C_Unimplemented (cpu *c)
{
	printf ("\nUNIMPLEMENTED INSTRUCTION %02x\n", C_GetByte (c, --c->pc));
	printf ("Instructions executed: %lu\n", c->instructions);
	printf ("Cycles: %lu\n", c->cycles);
	exit (1);
}

void C_Emulate (cpu *c, uint8_t opcode)
{
	/* READ HEADER FILES FOR INSTRUCTION DOCUMENTATION */
	c->instructions++;
	c->cycles += C_CYCLES[opcode];

	switch (opcode)
	{
		// NOP
		case 0x00: break;
		case 0x08: break;
		case 0x10: break;
		case 0x18: break;
		case 0x20: break;
		case 0x28: break;
		case 0x30: break;
		case 0x38: break; // nop

		case 0xcb: jmp (c, true, ARG16); break;
		case 0xdd: call (c, true, ARG16); break;
		case 0xed: call (c, true, ARG16); break;
		case 0xfd: call (c, true, ARG16); break;
		case 0xd9: ret (c); break;

		// CARRY
		case 0x37: c->flag_c = 1; break;
		case 0x2f: c->a = ~c->a; break;
		case 0x3f: c->flag_c ^= 1; break;

		// JUMP
		case 0xe9: c->pc = C_GetHL (c); break;

		case 0xc3: jmp (c, true, ARG16); break;
		case 0xda: jmp (c, c->flag_c, ARG16); break;
		case 0xd2: jmp (c, !c->flag_c, ARG16); break;
		case 0xca: jmp (c, c->flag_z, ARG16); break;
		case 0xc2: jmp (c, !c->flag_z, ARG16); break;
		case 0xfa: jmp (c, c->flag_s, ARG16); break;
		case 0xf2: jmp (c, !c->flag_s, ARG16); break;
		case 0xea: jmp (c, c->flag_p, ARG16); break;
		case 0xe2: jmp (c, !c->flag_p, ARG16); break;

		// CALL
		case 0xcd: call (c, true, ARG16); break;
		case 0xdc: call (c, c->flag_c, ARG16); break;
		case 0xd4: call (c, !c->flag_c, ARG16); break;
		case 0xcc: call (c, c->flag_z, ARG16); break;
		case 0xc4: call (c, !c->flag_z, ARG16); break;
		case 0xfc: call (c, c->flag_s, ARG16); break;
		case 0xf4: call (c, !c->flag_s, ARG16); break;
		case 0xec: call (c, c->flag_p, ARG16); break;
		case 0xe4: call (c, !c->flag_p, ARG16); break;

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

		// IMMEDIATE
		case 0x01:
			C_SetBC (c, ARG16);
			PC2;
			break; // LXI B
		case 0x11:
			C_SetDE (c, ARG16);
			PC2;
			break; // LXI D
		case 0x21:
			C_SetHL (c, ARG16);
			PC2;
			break; // LXI H
		case 0x31:
			c->sp = ARG16;
			PC2;
			break; // LXI SP

		case 0x06:
			c->b = ARG8;
			PC1;
			break; // MVI B
		case 0x0e:
			c->c = ARG8;
			PC1;
			break; // MVI C
		case 0x16:
			c->d = ARG8;
			PC1;
			break; // MVI D
		case 0x1e:
			c->e = ARG8;
			PC1;
			break; // MVI E
		case 0x26:
			c->h = ARG8;
			PC1;
			break; // MVI H
		case 0x2e:
			c->l = ARG8;
			PC1;
			break; // MVI L
		case 0x36:
			C_SetByte (c, C_GetHL (c), ARG8);
			PC1;
			break; // MVI M
		case 0x3e:
			c->a = ARG8;
			PC1;
			break;

		case 0xc6: adi (c); break;
		case 0xce: aci (c); break;
		case 0xd6: sui (c); break;
		case 0xde: sbi (c); break;
		case 0xe6: ani (c); break;
		case 0xee: xri (c); break;
		case 0xf6: ori (c); break;
		case 0xfe: cpi (c); break;

		// DATA TRANSFER
		case 0x0a: c->a = C_DerefBC (c); break;				// LDAX B
		case 0x1a: c->a = C_DerefDE (c); break;				// LDAX D
		case 0x40: c->b = c->b; break;						// MOV B, B
		case 0x41: c->b = c->c; break;						// MOV B, C
		case 0x42: c->b = c->d; break;						// MOV B, D
		case 0x43: c->b = c->e; break;						// MOV B, E
		case 0x44: c->b = c->h; break;						// MOV B, H
		case 0x45: c->b = c->l; break;						// MOV B, L
		case 0x46: c->b = C_DerefHL (c); break;				// MOV B, M
		case 0x47: c->b = c->a; break;						// MOV B, A
		case 0x48: c->c = c->b; break;						// MOV C, B
		case 0x49: c->c = c->c; break;						// MOV C, C
		case 0x4a: c->c = c->d; break;						// MOV C, D
		case 0x4b: c->c = c->e; break;						// MOV C, E
		case 0x4c: c->c = c->h; break;						// MOV C, H
		case 0x4d: c->c = c->l; break;						// MOV C, L
		case 0x4e: c->c = C_DerefHL (c); break;				// MOV C, M
		case 0x4f: c->c = c->a; break;						// MOV C, A
		case 0x50: c->d = c->b; break;						// MOV D, B
		case 0x51: c->d = c->c; break;						// MOV D, C
		case 0x52: c->d = c->d; break;						// MOV D, D
		case 0x53: c->d = c->e; break;						// MOV D, E
		case 0x54: c->d = c->h; break;						// MOV D, H
		case 0x55: c->d = c->l; break;						// MOV D, L
		case 0x56: c->d = C_DerefHL (c); break;				// MOV D, M
		case 0x57: c->d = c->a; break;						// MOV D, A
		case 0x58: c->e = c->b; break;						// MOV E, B
		case 0x59: c->e = c->c; break;						// MOV E, C
		case 0x5a: c->e = c->d; break;						// MOV E, D
		case 0x5b: c->e = c->e; break;						// MOV E, E
		case 0x5c: c->e = c->h; break;						// MOV E, H
		case 0x5d: c->e = c->l; break;						// MOV E, L
		case 0x5e: c->e = C_DerefHL (c); break;				// MOV E, M
		case 0x5f: c->e = c->a; break;						// MOV E, A
		case 0x60: c->h = c->b; break;						// MOV H, B
		case 0x61: c->h = c->c; break;						// MOV H, C
		case 0x62: c->h = c->d; break;						// MOV H, D
		case 0x63: c->h = c->e; break;						// MOV H, E
		case 0x64: c->h = c->h; break;						// MOV H, H
		case 0x65: c->h = c->l; break;						// MOV H, L
		case 0x66: c->h = C_DerefHL (c); break;				// MOV H, M
		case 0x67: c->h = c->a; break;						// MOV H, A
		case 0x68: c->l = c->b; break;						// MOV L, B
		case 0x69: c->l = c->c; break;						// MOV L, C
		case 0x6a: c->l = c->d; break;						// MOV L, D
		case 0x6b: c->l = c->e; break;						// MOV L, E
		case 0x6c: c->l = c->h; break;						// MOV L, H
		case 0x6d: c->l = c->l; break;						// MOV L, L
		case 0x6e: c->l = C_DerefHL (c); break;				// MOV L, M
		case 0x6f: c->l = c->a; break;						// MOV L, A
		case 0x70: C_SetByte (c, C_GetHL (c), c->b); break; // MOV M, B
		case 0x71: C_SetByte (c, C_GetHL (c), c->c); break; // MOV M, C
		case 0x72: C_SetByte (c, C_GetHL (c), c->d); break; // MOV M, D
		case 0x73: C_SetByte (c, C_GetHL (c), c->e); break; // MOV M, E
		case 0x74: C_SetByte (c, C_GetHL (c), c->h); break; // MOV M, H
		case 0x75: C_SetByte (c, C_GetHL (c), c->l); break; // MOV M, L
		case 0x77: C_SetByte (c, C_GetHL (c), c->a); break; // MOV M, A
		case 0x78: c->a = c->b; break;						// MOV A, B
		case 0x79: c->a = c->c; break;						// MOV A, C
		case 0x7a: c->a = c->d; break;						// MOV A, D
		case 0x7b: c->a = c->e; break;						// MOV A, E
		case 0x7c: c->a = c->h; break;						// MOV A, H
		case 0x7d: c->a = c->l; break;						// MOV A, L
		case 0x7e: c->a = C_DerefHL (c); break;				// MOV A, M
		case 0x7f: c->a = c->a; break;						// MOV A, A

		case 0x02: C_SetByte (c, C_GetBC (c), c->a); break; // STAX B
		case 0x12:
			C_SetByte (c, C_GetDE (c), c->a);
			break; // STAX D

		// REGISTER PAIR
		case 0xc5: S_Push (c, C_GetBC (c)); break; // PUSH B
		case 0xd5: S_Push (c, C_GetDE (c)); break; // PUSH D
		case 0xe5: S_Push (c, C_GetHL (c)); break; // PUSH H
		case 0xf5: S_PushPSW (c); break;		   // PUSH PSW

		case 0xc1: C_SetBC (c, S_Pop (c)); break; // POP B
		case 0xd1: C_SetDE (c, S_Pop (c)); break; // POP D
		case 0xe1: C_SetHL (c, S_Pop (c)); break; // POP H
		case 0xf1: S_PopPSW (c); break;			  // POP PSW

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

		// SINGLE REGISTER
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
		case 0x27: daa (c); break;

		// ROTATE ACCUMULATOR
		case 0x07: rlc (c); break;
		case 0x0f: rrc (c); break;
		case 0x1f: rar (c); break;
		case 0x17: ral (c); break;

		// I/O
		case 0xdb: in (c); break;
		case 0xd3: out (c); break;
		case 0x76: hlt (c); break;

		// REGISTER OR MEMORY TO ACCUMULATOR
		case 0x87: add (c, REG (a)); break;
		case 0x80: add (c, REG (b)); break;
		case 0x81: add (c, REG (c)); break;
		case 0x82: add (c, REG (d)); break;
		case 0x83: add (c, REG (e)); break;
		case 0x84: add (c, REG (h)); break;
		case 0x85: add (c, REG (l)); break;
		case 0x86: add_m (c); break;
		case 0x8f: adc (c, REG (a)); break;
		case 0x88: adc (c, REG (b)); break;
		case 0x89: adc (c, REG (c)); break;
		case 0x8a: adc (c, REG (d)); break;
		case 0x8b: adc (c, REG (e)); break;
		case 0x8c: adc (c, REG (h)); break;
		case 0x8d: adc (c, REG (l)); break;
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
		case 0xb8: cmp (c, REG (b)); break;
		case 0xb9: cmp (c, REG (c)); break;
		case 0xba: cmp (c, REG (d)); break;
		case 0xbb: cmp (c, REG (e)); break;
		case 0xbc: cmp (c, REG (h)); break;
		case 0xbd: cmp (c, REG (l)); break;
		case 0xbe: cmp_m (c); break;
		case 0xbf: cmp (c, REG (a)); break;

		//
		// 3 opcodes
		//

		// DIRECT ADDRESSING
		case 0x32:
			C_SetByte (c, ARG16, c->a);
			PC2;
			break; // STA adr
		case 0x3a:
			c->a = C_GetByte (c, ARG16);
			PC2;
			break; // LDA adr
		case 0x22:
			C_SetWord (c, ARG16, C_GetHL (c));
			PC2;
			break; // SHLD adr
		case 0x2a:
			C_SetHL (c, C_GetWord (c, ARG16));
			PC2;
			break; // LHLD adr

		// INTERRUPT TOGGLE
		case 0xfb: c->interrupts_enabled = true; break; // EI
		case 0xf3:
			c->interrupts_enabled = false;
			break; // DI

		// RST
		case 0xc7: call (c, true, 0x0); break;
		case 0xcf: call (c, true, 0x8); break;
		case 0xd7: call (c, true, 0x10); break;
		case 0xdf: call (c, true, 0x18); break;
		case 0xe7: call (c, true, 0x20); break;
		case 0xef: call (c, true, 0x28); break;
		case 0xf7: call (c, true, 0x30); break;
		case 0xff: call (c, true, 0x38); break;

		default: C_Unimplemented (c); break;
	}
}
