#pragma once

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "cpu.h"
#include "sys.h"
#include "debug.h"
#include "render.h"
#include "instructions.h"

#define ANSI_COLOR_RED	 "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define WINDOW_WIDTH	 224
#define WINDOW_HEIGHT	 256
#define WINDOW_TITLE	 "8080 Emulator (unstable)"
#define WINDOW_SCALE	 2
