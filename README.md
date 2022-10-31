# intel8080

An Intel 8080 emulator written in c99.

<p align="center">
  <img src="https://github.com/ronshabi/8080C/blob/master/doc/screenshots/8080c-screenshot-demo2.png?raw=true" width="300" alt="8080C emulating Space Invaders - main menu"/>
  <img src="https://github.com/ronshabi/8080C/blob/master/doc/screenshots/8080c-screenshot-demo1.png?raw=true" width="300" alt="8080C emulating Space Invaders - game view"/>
  <p align="center">Emulating Space Invaders</p>
</p>

## Controls
`q` - Exit

`c` - Insert coin

`z` - Pause *(added as a debug feature, also pauses the disassembly -- not avaliable in original 8080)*

`Space` - Fire

`Arrow keys` - P1/P2 Move

`1` - Start a 1 player game

`2` - Start a 2 player game


## Build
Create a `build` directory and run: `cmake -B. -S <SOURCE_PATH> && make`

### Dependencies
- SDL2


## Debugging
`debug.h` is your goto file to apply debug macros!
### Activating the disassembler
Enable `DEBUG_MODE_REGULAR` in `debug.h`
### Stopping at instruction
Enable `DEBUG_MODE_STOP` in `debug.h`, edit `DEBUG_MODE_STOP_AT_INSTRUCTION` to your liking.


## Creating tests
In order to make the process of CPU testing a little bit easier, use `create_new_test.py` in `scripts`: `python3 create_new_test.py <TEST_NAME>`

This will generate a `test_<TEST_NAME>.c` file in `tests/` with `test_base.c` as the template.

## Author
Ron Shabi

## Useful resources
[ComputerArcheology - hardware specifications & super helpful disassembly](http://computerarcheology.com/Arcade/SpaceInvaders/)

[Lazyfoo's SDL tutorials](https://lazyfoo.net/tutorials/SDL/)

[Emulator101 opcode table & general reference](http://www.emulator101.com/reference/8080-by-opcode.html)


## License
MIT
