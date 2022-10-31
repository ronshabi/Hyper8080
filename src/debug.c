#include "defs.h"

void D_Address(uint16_t addr)
{
    #ifdef DEBUG_MODE_REGULAR
    printf(" $%04x", addr);
    #endif
}
