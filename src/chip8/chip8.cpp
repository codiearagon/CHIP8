#include <iostream>
#include "chip8.h"

Chip8::Chip8() {
    memory = new Memory();
}

Chip8::~Chip8() {
    delete memory;
}

void Chip8::run() {
    std::cout << "Hello world!" << std::endl;

    memory->write(0, 10);
    for(int i = 0; i < 10; i++) {
        std::cout << i << ": " << (int)memory->read(i) << std::endl;
    }
}