#include <iostream>
#include <SDL2/SDL.h>
#include "chip8.h"

int main(int argc, char **argv) {
    Chip8 chip8;
    chip8.run();
    return 0;
}