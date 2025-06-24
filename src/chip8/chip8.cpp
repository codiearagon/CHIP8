#include <iostream>
#include <fstream>
#include "chip8.h"

Chip8::Chip8() {
    memory = new Memory();

    pc = 0x0;
}

Chip8::~Chip8() {
    delete memory;
}

void Chip8::loadRom(std::string fileName) {
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);

    if(!file) {
        std::cerr << "Failed to open file\n";
        return;
    }
    
    std::streamsize size = file.tellg();
    char* buffer = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    for(long i = 0; i < size; ++i) {
        memory->write(0x200 + i, buffer[i]);
    }

    delete[] buffer;
}

uint16_t Chip8::fetch() {
    uint8_t inst1 = memory->read(pc);
    uint8_t inst2 = memory->read(pc + 1);
    uint16_t instruction = (inst1 << 8) | inst2;
    pc += 2;

    return instruction;
}

void Chip8::decode() {
    uint16_t instruction = fetch();
    uint16_t opcode = (instruction & 0xF000) >> 12;
    uint16_t secondNib = (instruction & 0x0F00) >> 8;
    uint16_t thirdNib = (instruction & 0x00F0) >> 4;
    uint16_t fourthNib = (instruction & 0x000F);

    switch(opcode) {
        case 0x0:
            break;
        case 0x1:
            pc = (instruction & 0x0FFF);
            break;
        case 0x6:
            registers[secondNib] = (instruction & 0x00FF);
            break;
        case 0x7:
            registers[secondNib] += (instruction & 0x00FF);
            break;
        case 0xA:
            ir = (instruction & 0x0FFF);
            break;
        case 0xD:

            break;
    }
}

void Chip8::run() {
    for(int i = 0x200; i < 0x300; i++) {
        std::cout << std::hex << i << ": " << (int)memory->read(i) << std::endl;
    }
}