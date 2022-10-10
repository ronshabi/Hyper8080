#pragma once
#include "defs.h"

long Sys_UnixTime (void);
void Sys_LoadROM (FILE *fptr, char *filename, unsigned char *memptr);
void Sys_MirrorRAM (unsigned char **ptr, uint16_t from, uint16_t howmuch);
void Sys_AllocateMemory (unsigned char **ptr, int howmuch);
void Sys_PollEvents (cpu *c, SDL_Event *e, bool *quit);