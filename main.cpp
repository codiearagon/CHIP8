#include <iostream>
#include <SDL2/SDL.h>
#include <chip8.h>

int main(int argc, char **argv) {
    std::cout << "Hello" << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    Chip8 chip8;
    chip8.run();
    return 0;
}