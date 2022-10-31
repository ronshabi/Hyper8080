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
    *ptr = calloc(howmuch, 1);
}