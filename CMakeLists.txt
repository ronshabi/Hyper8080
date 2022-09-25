cmake_minimum_required(VERSION 3.23)
project(8080_Emu C)

set(CMAKE_C_STANDARD 99)

add_executable(8080_Emu src/main.c src/cpu.c src/cpu.h src/memory.c src/memory.h)
