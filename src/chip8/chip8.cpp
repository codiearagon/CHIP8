#include <iostream>
#include <fstream>
#include "chip8.h"

Chip8::Chip8() {
    memory = new Memory();
    pc = 0x200;
}

Chip8::~Chip8() {
    delete memory;
    delete display;
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
    std::cout << std::hex << "PC: " << (pc - 2) << " inst: " << instruction << std::endl;
    uint16_t opcode = (instruction & 0xF000) >> 12;
    uint16_t secondNib = (instruction & 0x0F00) >> 8;
    uint16_t thirdNib = (instruction & 0x00F0) >> 4;
    uint16_t fourthNib = (instruction & 0x000F);

    switch(opcode) {
        case 0x0:
            if((instruction & 0x0FFF) == 0x00E0)
                display->clearDisplay();
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
            display->draw(registers, secondNib, thirdNib, fourthNib, ir); // display tick
            break;
    }
}

void Chip8::run() {
    display = new Display(memory);

    bool running = true;
    SDL_Event event;

    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        tick(); // cpu tick

        SDL_Delay(5);
    }

    display->destroy();
}

void Chip8::tick() {
    decode();
}