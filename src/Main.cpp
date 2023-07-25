
#include <SDL2/SDL.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "cpu.h"
#include "debug.h"
#include "instructions_C.h"
#include "render.h"
#include "sys.h"

constexpr auto window_height = 256;
constexpr auto window_width = 224;
constexpr auto window_scale = 2;
constexpr auto window_title = "Hyper8080 v0.4";


int main(int argc, char *argv[]) {
    //
    // Check arguments
    //
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file...>\n", argv[0]);
        exit(1);
    }

    FILE *f;
    cpu c;
    unsigned char *buffer;
    uint8_t c_currentOpcode;
    bool quit = false;
    SDL_Window *Window = NULL;
    SDL_Renderer *Renderer = NULL;
    uint64_t now = 0;
    uint64_t ms_Interrupt_Last = 0;
    uint64_t ms_Input_Last = 0;
    uint64_t ms_ClockSpeedMeasure_Last = 0;
    const uint8_t *keyboard = SDL_GetKeyboardState(NULL);

    int interrupt = 1;

    uint64_t cyclesLast = 0;
    uint64_t cyclesNow = 0;

    Sys_AllocateMemory(&buffer, 65535);
    Sys_LoadROM(f, argv[1], buffer);

    C_Init(&c);
    c.memory = buffer;

    //
    // Create window
    //
    R_Init();
    R_CreateWindow(&Window, &Renderer, window_title, window_width,
                   window_height, window_scale);
    R_ClearScreen(&Renderer);
    R_Update(&Renderer);

    //
    // Event loop
    //
    while (!quit) {

        now = SDL_GetTicks();

        if ((double) (now - ms_Interrupt_Last) > HZ(120)) {
            if (interrupt ^= 1) {
                R_Render(&c, 0x2400, &Renderer);
                // Send RST 2
                C_GenerateInterrupt(&c, 0x10);
            } else {
                // Send RST 1
                C_GenerateInterrupt(&c, 0x08);
            }
            ms_Interrupt_Last = now;
        }

        if ((double) (now - ms_Input_Last) > HZ(30)) {
            Sys_HandleInputs(&c, &keyboard, &quit);
            ms_Input_Last = now;
        }

        cyclesNow = c.cycles;
        if (now - ms_ClockSpeedMeasure_Last > 1000) {
            printf("Running @ %.2f MHz\n", (cyclesNow - cyclesLast) / 10E6);
            cyclesLast = cyclesNow;
            ms_ClockSpeedMeasure_Last = now;
        }

        if (!c.paused) {
            D_Disasm(&c);

            c_currentOpcode = C_GetByte(&c, c.pc);
            c.pc += 1;
            C_Emulate(&c, c_currentOpcode);

            D_StopHandler(&c, &quit);
            D_Newline;
        }
    }

    D_Summary(&c);

    R_Exit(&Window, &Renderer);
    free(buffer);
    return 0;
}
