#include <iostream>
#include "chip8.h"

int main(int argc, char **argv) {
    Chip8 chip8;
    // chip8.loadRom("roms/ibmlogo.ch8");
    // chip8.loadRom("roms/test_opcode.ch8");
    // chip8.loadRom("roms/bc_test.ch8");
    // chip8.loadRom("roms/tetris");
    // chip8.loadRom("roms/invaders");
    chip8.loadRom("roms/brick.ch8");
    chip8.selectMode(chip8.MODERN);
    chip8.run();
    return 0;
}