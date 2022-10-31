#pragma once

//
// useful debugging macros
//

// #define DEBUG_MODE_REGULAR
// #define DEBUG_MODE_STOP
#define DEBUG_MODE_STOP_AT_INSTRUCTION 2000000
#define PrintTime(time) printf("Time: %lu\n", time)
#define println(msg) printf("%s\n", msg)
#define printLong(l) printf("%lu\n", l)

void D_Address(uint16_t addr);
