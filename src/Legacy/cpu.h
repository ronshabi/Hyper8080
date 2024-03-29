#pragma once
#define PC1 c->pc += 1
#define PC2 c->pc += 2

// Quickly access register location
#define REG(reg) &(c->reg)

// Quickly get immediate
#define ARG8 C_GetByte(c, c->pc)
#define ARG16 C_GetWord(c, c->pc)

// Twos-compliment flip sign
#define FLIP(n) (~(n) + 1)

// Device numbers
// Output
#define DEVICE_SHIFT_AMT 2
#define DEVICE_SOUND1 3
#define DEVICE_SHIFT_DATA 4
#define DEVICE_SOUND2 5
#define DEVICE_WATCHDOG 6

// Input
#define DEVICE_INP0 0
#define DEVICE_INP1 1
#define DEVICE_INP2 2
#define DEVICE_SHIFT_IN 3

typedef struct cpu {
    uint8_t a, b, c, d, e, h, l;
    uint16_t pc, sp;
    uint8_t *memory;

    // condition
    uint64_t cycles, instructions;
    bool halt, interrupts_enabled;

    // flags
    bool flag_z, flag_s, flag_p, flag_c, flag_ac;

    // iX - input port X, oX - output port X
    uint8_t i0, i1, i2, i3, o2, o3, o4, o5, o6;

    // bitshift register
    uint16_t shift;
    uint8_t shift_amt;

    // debugging tools
    uint8_t paused;

} cpu;

// Core
void C_Init(cpu *c);

void C_SetMemory(cpu *c, uint8_t *memory_ptr);

// Memory
uint8_t C_GetByte(cpu *c, uint16_t address);

uint16_t C_GetWord(cpu *c, uint16_t address);

void C_SetByte(cpu *c, uint16_t address, uint8_t val);

void C_SetWord(cpu *c, uint16_t address, uint16_t val);

// Register pairs
uint16_t C_GetBC(cpu *c);

uint16_t C_GetDE(cpu *c);

uint16_t C_GetHL(cpu *c);

void C_SetBC(cpu *c, uint16_t val);

void C_SetDE(cpu *c, uint16_t val);

void C_SetHL(cpu *c, uint16_t val);

uint8_t C_DerefBC(cpu *c);

uint8_t C_DerefDE(cpu *c);

uint8_t C_DerefHL(cpu *c);

uint8_t C_DerefSP(cpu *c, uint16_t offset);

// Flags
uint8_t C_Flags_Get(cpu *c);

void C_Flags_SetZSP(cpu *c, uint8_t val);

void C_Flags_SetCarryFromWord(cpu *c, uint16_t num);

// Stack
void S_Push(cpu *c, uint16_t word);

uint16_t S_Pop(cpu *c);

void S_PushPSW(cpu *c);

void S_PopPSW(cpu *c);

// Emulation
void C_Emulate(cpu *c, uint8_t opcode);

void C_Unimplemented(cpu *c);

void C_GenerateInterrupt(cpu *c, uint16_t addr);
