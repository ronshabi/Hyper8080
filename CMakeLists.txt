cmake_minimum_required(VERSION 3.1)
project(8080_Emu C)

set(CMAKE_C_STANDARD 99)

add_executable(8080_Emu src/main.c src/cpu.c src/cpu.h)

# Testing
enable_testing()
add_executable(test_stack tests/test_stack.c)
add_executable(test_stack_overflowing tests/test_stack_overflowing.c)
add_executable(test_cpu_basic_memory tests/test_cpu_basic_memory.c)
add_executable(test_flags tests/test_flags.c)
add_test(Test-Stack-Push-Pop test_stack)
add_test(Test-Stack-Overflowing test_stack_overflowing)
add_test(Test-CPU-Basic-Memory-Operations test_cpu_basic_memory)
add_test(Test-Flags test_flags)