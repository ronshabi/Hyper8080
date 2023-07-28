# Hyper8080 - an Intel 8080 CPU emulator, written in C++20

![Hyper8080-logo.png](docs/Hyper8080-logo.png)

# About
The Hyper8080 emulator aims to make programs written with the Intel 8080 ISA run on
modern hardware across all operating systems, supporting text and graphical mode.

Originally, I wrote this emulator in C99, and decided to improve its performance and code
quality by applying multithreading and utilizing modern C++ features.

The project is still in development, therefore bugs may occur.
If you want to report an issue or contribute to the project - pull requests are welcome.

## Dependencies
- SDL2
- spdlog

## Build
This project uses CMake, and requires no extra configuration in order to build.
Simply ```cmake ``` from your build directory, and then build with your preferred generator.

## Author
Ron Shabi (ron@ronsh.net)

## Useful resources
[ComputerArcheology - hardware specifications & super helpful disassembly](http://computerarcheology.com/Arcade/SpaceInvaders/)

[Lazyfoo's SDL tutorials](https://lazyfoo.net/tutorials/SDL/)

[Emulator101 opcode table & general reference](http://www.emulator101.com/reference/8080-by-opcode.html)


## License
MIT
