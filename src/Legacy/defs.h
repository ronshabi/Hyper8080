#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cpu.h"
#include "debug.h"
#include "instructions_C.h"
#include "render.h"
#include "sys.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define WINDOW_WIDTH 224
#define WINDOW_HEIGHT 256
#define WINDOW_TITLE "intel8080 (stable-0.3)"
#define WINDOW_SCALE 2
