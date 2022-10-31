#pragma once
#include "defs.h"

#define HZ(n) (double)(1000.0 / n)

void Sys_LoadROM(FILE *fptr, char *filename, unsigned char *memptr);
void Sys_AllocateMemory(unsigned char **ptr, int howmuch);
