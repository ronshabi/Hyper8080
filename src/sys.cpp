#include "sys.h"

void Sys_LoadROM(FILE *fptr, char *filename, unsigned char *memptr) {
    fptr = fopen(filename, "rb"); // (try to) open file

    // Check if file exists, if not -> exit and print error (with nice colors)
    if (fptr == NULL) {
        fprintf(stderr, ANSI_COLOR_RED);
        fprintf(stderr, "fatal error: ");
        fprintf(stderr, ANSI_COLOR_RESET);
        fprintf(stderr, "Couldn't open file %s: file doesn't exist.\n",
                filename);
        exit(1);
    }

    // get file size in bytes
    fseek(fptr, 0L, SEEK_END);
    long fsize = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    fread(memptr, fsize, 1, fptr); // Read to memory buffer
    fclose(fptr);                  // File handle not needed anymore
}

void Sys_AllocateMemory(unsigned char **ptr, int howmuch) {
    *ptr = (unsigned char*)(calloc(howmuch, 1));
}

void Sys_HandleInputs(cpu *c, const uint8_t **keyboard, bool *quitcond) {
    // Reset inputs instead of handling keyup events
    c->i0 &= 0b10001111;
    c->i1 &= 0b10001000;
    c->i2 &= 0b10001011;

    SDL_PumpEvents();

    if ((*keyboard)[SDL_SCANCODE_Q]) {
        // Quit
        *quitcond = true;
    }

    if ((*keyboard)[SDL_SCANCODE_C]) {
        // Insert coin
        c->i1 |= 1;
    }

    if ((*keyboard)[SDL_SCANCODE_Z]) {
        // Pause (for debugging, not implemented in the 8080 itself)
        c->paused ^= 1;
    }
    if ((*keyboard)[SDL_SCANCODE_1]) {
        // P1 start
        c->i1 |= 0x4;
    }
    if ((*keyboard)[SDL_SCANCODE_1]) {
        // P2 start
        c->i1 |= 0x2;
    }

    if ((*keyboard)[SDL_SCANCODE_SPACE]) {
        // Player 1/2 shoot
        c->i0 |= 0x10;
        c->i1 |= 0x10;
        c->i2 |= 0x10;
    }

    if ((*keyboard)[SDL_SCANCODE_LEFT]) {
        // Player 1/2 left
        c->i0 |= 0x20;
        c->i1 |= 0x20;
        c->i2 |= 0x20;
    }

    if ((*keyboard)[SDL_SCANCODE_RIGHT]) {
        // Player 1/2 right
        c->i0 |= 0x40;
        c->i1 |= 0x40;
        c->i2 |= 0x40;
    }
}
