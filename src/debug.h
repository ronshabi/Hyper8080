#pragma once

#define DEBUG_MODE_REGULAR
#define DEBUG_MODE_STOP
#define DEBUG_MODE_STOP_AT_INSTRUCTION 2000000

#ifdef DEBUG_MODE_REGULAR
#define D_Newline printf("\n")
#else
#define D_Newline
#endif

static const uint8_t C_CYCLES[] = {
    4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,
    5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 16, 5,  5,  5,
    7,  4,  4,  10, 16, 5,  5,  5,  7,  4,  4,  10, 13, 5,  10, 10, 10, 4,  4,
    10, 13, 5,  5,  5,  7,  4,  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,
    5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,
    5,  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,  7,  7,
    7,  7,  7,  7,  7,  7,  5,  5,  5,  5,  5,  5,  7,  5,  4,  4,  4,  4,  4,
    4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,
    4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,
    7,  4,  5,  10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 11, 7,  11, 5,
    10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 18,
    11, 11, 7,  11, 5,  5,  10, 5,  11, 11, 7,  11, 5,  10, 10, 4,  11, 11, 7,
    11, 5,  5,  10, 4,  11, 11, 7,  11,
};

static const char *C_INSTRUCTIONS[] = {
    "NOP",     "LXI B",   "STAX B",  "INX B",   "INR B",   "DCR B",
    "MVI B",   "RLC",     "NOP",     "DAD B",   "LDAX B",  "DCX B",
    "INR C",   "DCR C",   "MVI C",   "RRC",     "NOP",     "LXI D",
    "STAX D",  "INX D",   "INR D",   "DCR D",   "MVI D",   "RALC",
    "NOP",     "DAD D",   "LDAX D",  "DCX D",   "INR E",   "DCR E",
    "MVI E",   "RAR",     "NOP",     "LXI H",   "SHLD",    "INX H",
    "INR H",   "DCR H",   "MVI H",   "DAA",     "NOP",     "DAD H",
    "LHLD",    "DCX H",   "INR L",   "DCR L",   "MVI L",   "CMA",
    "NOP",     "LXI SP",  "STA",     "INX SP",  "INR M",   "DCR M",
    "MVI M",   "STC",     "NOP",     "DAD SP",  "LDA",     "DCX SP",
    "INR A",   "DCR A",   "MVI A",   "CMC",     "MOV B,B", "MOV B,C",
    "MOV B,D", "MOV B,E", "MOV B,H", "MOV B,L", "MOV B,M", "MOV B,A",
    "MOV C,B", "MOV C,C", "MOV C,D", "MOV C,E", "MOV C,H", "MOV C,L",
    "MOV C,M", "MOV C,A", "MOV D,B", "MOV D,C", "MOV D,D", "MOV D,E",
    "MOV D,H", "MOV D,L", "MOV D,M", "MOV D,A", "MOV E,B", "MOV E,C",
    "MOV E,D", "MOV E,E", "MOV E,H", "MOV E,L", "MOV E,M", "MOV E,A",
    "MOV H,B", "MOV H,C", "MOV H,D", "MOV H,E", "MOV H,H", "MOV H,L",
    "MOV H,M", "MOV H,A", "MOV L,B", "MOV L,C", "MOV L,D", "MOV L,E",
    "MOV L,H", "MOV L,L", "MOV L,M", "MOV L,A", "MOV M,B", "MOV M,C",
    "MOV M,D", "MOV M,E", "MOV M,H", "MOV M,L", "HLT",     "MOV M,A",
    "MOV A,B", "MOV A,C", "MOV A,D", "MOV A,E", "MOV A,H", "MOV A,L",
    "MOV A,M", "MOV A,A", "ADD B",   "ADD C",   "ADD D",   "ADD E",
    "ADD H",   "ADD L",   "ADD M",   "ADD A",   "ADC B",   "ADC C",
    "ADC D",   "ADC E",   "ADC H",   "ADC L",   "ADC M",   "ADC A",
    "SUB B",   "SUB C",   "SUB D",   "SUB E",   "SUB H",   "SUB L",
    "SUB M",   "SUB A",   "SBB B",   "SBB C",   "SBB D",   "SBB E",
    "SBB H",   "SBB L",   "SBB M",   "SBB A",   "ANA B",   "ANA C",
    "ANA D",   "ANA E",   "ANA H",   "ANA L",   "ANA M",   "ANA A",
    "XRA B",   "XRA C",   "XRA D",   "XRA E",   "XRA H",   "XRA L",
    "XRA M",   "XRA A",   "ORA B",   "ORA C",   "ORA D",   "ORA E",
    "ORA H",   "ORA L",   "ORA M",   "ORA A",   "CMP B",   "CMP C",
    "CMP D",   "CMP E",   "CMP H",   "CMP L",   "CMP M",   "CMP A",
    "RNZ",     "POP B",   "JNZ",     "JMP",     "CNZ",     "PUSH B",
    "ADI",     "RST 0",   "RZ",      "RET",     "JZ",      "NOP",
    "CZ",      "CALL",    "ACI",     "RST 1",   "RNC",     "POP D",
    "JNC",     "OUT",     "CNC",     "PUSH D",  "SUI",     "RST 2",
    "RC",      "NOP",     "JC",      "IN",      "CC",      "NOP",
    "SBI",     "RST 3",   "RPO",     "POP H",   "JPO",     "XTHL",
    "CPO",     "PUSH H",  "ANI",     "RST 4",   "RPE",     "PCHL",
    "JPE",     "XCHG",    "CPE",     "NOP",     "XRI",     "RST 5",
    "RP",      "POP PSW", "JP",      "DI",      "CP",      "PUSH PSW",
    "ORI",     "RST 6",   "RM",      "SPHL",    "JM",      "EI",
    "CM",      "NOP",     "CPI",     "RST 7"};

void D_Address(uint16_t addr);
void D_Device(uint8_t dev);
void D_GlobalMessage(const char* msg);
void D_Disasm(cpu *c);
void D_StopHandler(cpu* c, bool *quit);
void D_Summary(cpu *c);

