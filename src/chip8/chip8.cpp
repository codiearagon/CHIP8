#include <iostream>
#include <fstream>
#include <string>
#include "chip8.h"

Chip8::Chip8() {
    gen = std::mt19937(rd());
    dist = std::uniform_int_distribution<uint8_t>(0, 255);

    memory = new Memory();
    mode = ORIGINAL;
    pc = 0x200;
}

Chip8::~Chip8() {
    delete audio;
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

void Chip8::selectMode(Mode _mode) {
    mode = _mode;
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
            else if((instruction & 0x0FFF) == 0x00EE) {
                pc = stack.top();
                stack.pop();
            }

            break;
        case 0x1:
            pc = (instruction & 0x0FFF);
            break;
        case 0x2:
            stack.push(pc);
            pc = (instruction & 0x0FFF);
            break;
        case 0x3:
            if(registers[secondNib] == (instruction & 0x00FF))
                pc += 2;
            break;
        case 0x4:
            if(registers[secondNib] != (instruction & 0x00FF))
                pc += 2;
            break;
        case 0x5:
            if(registers[secondNib] == registers[thirdNib])
                pc += 2;
            break;
        case 0x6:
            registers[secondNib] = (instruction & 0x00FF);
            break;
        case 0x7:
            registers[secondNib] += (instruction & 0x00FF);
            break;
        case 0x8:
            switch(fourthNib) { // logical and arithmetic operations
                case 0x0:
                    registers[secondNib] = registers[thirdNib];
                    break;
                case 0x1:
                    registers[secondNib] |= registers[thirdNib];
                    break;
                case 0x2:
                    registers[secondNib] &= registers[thirdNib];
                    break;
                case 0x3:
                    registers[secondNib] ^= registers[thirdNib];
                    break;
                case 0x4:
                    registers[secondNib] += registers[thirdNib];
                    break;
                case 0x5:
                    registers[0xF] = registers[secondNib] > registers[thirdNib] ? 1 : 0;
                    registers[secondNib] = registers[secondNib] - registers[thirdNib]; // vx = vx - vy
                    break;
                case 0x6:
                    if(mode == ORIGINAL)
                        registers[secondNib] = registers[thirdNib];
                    
                    registers[0xF] = (registers[secondNib] & 0x1); // set carry flag to LSB
                    registers[secondNib] >>= 1;

                    break;
                case 0x7:
                    registers[0xF] = registers[thirdNib] > registers[secondNib] ? 1 : 0;
                    registers[secondNib] = registers[thirdNib] - registers[secondNib]; // vx = vy - vx
                    break;
                case 0xE:
                    if(mode == ORIGINAL)
                        registers[secondNib] = registers[thirdNib];
                    
                    registers[0xF] = (registers[secondNib] & 0x80) >> 7; // set carry flag to MSB
                    registers[secondNib] <<= 1;

                    break;
            }

            break;
        case 0x9:
            if(registers[secondNib] != registers[thirdNib])
                pc += 2;
            break;
        case 0xA:
            ir = (instruction & 0x0FFF);
            break;
        case 0xB:
            if(mode == ORIGINAL)
                pc = (instruction & 0x0FFF) + registers[0];
            else
                pc = (instruction & 0x0FFF) + registers[secondNib];

            break;
        case 0xC:
            registers[secondNib] = dist(gen) & (instruction & 0x00FF);
            break;
        case 0xD:
            display->draw(registers, secondNib, thirdNib, fourthNib, ir); // display tick
            break;
        case 0xE:
            switch(instruction & 0x00FF) {
                case 0x9E:
                    if(keypad[registers[secondNib]])
                        pc += 2;
                    break;
                case 0xA1:
                    if(!keypad[registers[secondNib]])
                        pc += 2;
                    break;
            }

            break;
        case 0xF:
            switch(instruction & 0x00FF) {
                case 0x07:
                    registers[secondNib] = delayTimer;
                    break;
                case 0x15:
                    delayTimer = registers[secondNib];
                    break;
                case 0x18:
                    soundTimer = registers[secondNib];
                    break;
                case 0x1E:
                    ir += registers[secondNib];

                    if (ir >= 0x1000)
                        registers[0xF] = 1;
                    break;
                case 0x0A:
                    for(int i = 0; i < 16; i++) {
                        if(keypad[i]) {
                            registers[secondNib] = i;
                            pc += 2;
                        }
                    }

                    pc -= 2;
                    break;
                case 0x29:
                    ir = 0x50 + registers[secondNib] * 5;
                    break;
                case 0x33:
                {
                    std::string n = std::to_string(registers[secondNib]);
                    std::cout << n << std::endl;
                    int counter = 0;
                    for(char c : n) {
                        int digit = c - '0';
                        memory->write(ir + counter, digit);
                        std::cout << memory->read(ir + counter) << std::endl;
                        counter++;
                    }
                }
                    break;
                case 0x55:
                    for(int i = 0; i <= secondNib; ++i) {
                        memory->write(ir + i, registers[i]);

                        if(mode == ORIGINAL)
                            ir++;
                    }
                    break;
                case 0x65:
                    for(int i = 0; i <= secondNib; ++i) {
                        registers[i] = memory->read(ir + i);

                        if(mode == ORIGINAL)
                            ir++;
                    }
                    break;
            }

            break;
    }
}

void Chip8::run() {
    display = new Display(memory);
    audio = new Audio();

    bool running = true;
    SDL_Event event;

    while(running) {
        handleEvents(event, running); // handle events and inputs
        decode(); // fetch and decode instruction
        updateTimers(); // update delay and sound timers
        SDL_Delay(2);
    }

    display->destroy();
}

void Chip8::handleEvents(SDL_Event& event, bool& running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
                case SDLK_1: keypad[0x1] = 1; break;
                case SDLK_2: keypad[0x2] = 1; break;
                case SDLK_3: keypad[0x3] = 1; break;
                case SDLK_4: keypad[0xC] = 1; break;
                case SDLK_q: keypad[0x4] = 1; break;
                case SDLK_w: keypad[0x5] = 1; break;
                case SDLK_e: keypad[0x6] = 1; break;
                case SDLK_r: keypad[0xD] = 1; break;
                case SDLK_a: keypad[0x7] = 1; break;
                case SDLK_s: keypad[0x8] = 1; break;
                case SDLK_d: keypad[0x9] = 1; break;
                case SDLK_f: keypad[0xE] = 1; break;
                case SDLK_z: keypad[0xA] = 1; break;
                case SDLK_x: keypad[0x0] = 1; break;
                case SDLK_c: keypad[0xB] = 1; break;
                case SDLK_v: keypad[0xF] = 1; break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch(event.key.keysym.sym) {
                case SDLK_1: keypad[0x1] = 0; break;
                case SDLK_2: keypad[0x2] = 0; break;
                case SDLK_3: keypad[0x3] = 0; break;
                case SDLK_4: keypad[0xC] = 0; break;
                case SDLK_q: keypad[0x4] = 0; break;
                case SDLK_w: keypad[0x5] = 0; break;
                case SDLK_e: keypad[0x6] = 0; break;
                case SDLK_r: keypad[0xD] = 0; break;
                case SDLK_a: keypad[0x7] = 0; break;
                case SDLK_s: keypad[0x8] = 0; break;
                case SDLK_d: keypad[0x9] = 0; break;
                case SDLK_f: keypad[0xE] = 0; break;
                case SDLK_z: keypad[0xA] = 0; break;
                case SDLK_x: keypad[0x0] = 0; break;
                case SDLK_c: keypad[0xB] = 0; break;
                case SDLK_v: keypad[0xF] = 0; break;
            }
        }
    }
}

void Chip8::updateTimers() {
    uint32_t currentTime = SDL_GetTicks();
    if(currentTime - lastTime >= (1000 / 60)) {
        if (delayTimer > 0) 
            delayTimer--;

        if (soundTimer > 0) {
            std::cout << "BEEP" << std::endl;
            audio->playAudio();
            soundTimer--;
        }

        lastTime = currentTime;
    }
}