#include <iostream>
#include "chip8.h"

int main(int argc, char **argv) {
    Chip8 chip8;
    chip8.loadRom("roms/ibmlogo.ch8");
    chip8.run();
    return 0;
}