#include <iostream>
#include "memory.h"

Memory::Memory() {
    memory.fill(0);
}

Memory::~Memory() {}

uint8_t Memory::read(uint16_t addr) const {
    return memory[addr];
}

void Memory::write(uint16_t addr, uint8_t value) {
    memory[addr] = value;
}