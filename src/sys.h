#ifndef HYPER8080_SYS_H
#define HYPER8080_SYS_H

#include "defs.h"

#define HZ(n) (double)(1000.0 / n)

void sys_load_rom(FILE *fptr, char *filename, unsigned char *memptr);
void sys_allocate(unsigned char **ptr, int howmuch);
void sys_keyboard(cpu *c, const uint8_t **keyboard, bool *quitcond);

#endif /* HYPER8080_SYS_H */