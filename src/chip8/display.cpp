#include <iostream>
#include "display.h"

Display::Display(Memory* _mem) {
    mem = _mem;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return;
    }

    window = SDL_CreateWindow(
        "CHIP 8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if(!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}

Display::~Display() {}

void Display::clearDisplay() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Display::draw(uint8_t x, uint8_t y, uint8_t n, uint16_t _ir) {
    void *pixels;
    int pitch = 1;

    SDL_LockTexture(texture, nullptr, &pixels, &pitch);

    for (int row = 0; row < n; ++row) {
        uint8_t data = mem->read(_ir + row);

        for(int col = 0; col < 8; ++col) {
            
            if(x >= SCREEN_WIDTH)
                break;

            x++;
        }

        y++;

        if(y >= SCREEN_HEIGHT)
            break;
    }
    
    SDL_UnlockTexture(texture);
    clearDisplay();
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Display::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}