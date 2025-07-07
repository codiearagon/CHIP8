#include <iostream>
#include "display.h"

Display::Display(Memory* _mem) {
    mem = _mem;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
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

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
}

Display::~Display() {}

void Display::clearDisplay() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Display::draw(uint8_t *rg, uint8_t _x, uint8_t _y, uint8_t n, uint16_t& _ir) {
    std::cout << "Drawing..." << std::endl;
    int x = rg[_x] % TEXTURE_WIDTH;
    int y = rg[_y] % TEXTURE_HEIGHT;
    rg[0xF] = 0;

    void *pixels;
    int pitch;

    SDL_LockTexture(texture, nullptr, &pixels, &pitch);

    uint32_t *px = (uint32_t*)pixels;

    for (int row = 0; row < n; row++) {
        int data = mem->read(_ir + row);
        
        for(int col = 0; col < 8; col++) {
            if((data & (0x80 >> col)) != 0) { // if pixel in sprite row is on
                int pxLoc = y * (pitch / 4) + x;

                if(px[pxLoc] == 0xFFFFFFFF)
                    rg[0xF] = 1;

                togglePixel(&px[pxLoc]);
            }
        
            x++;
            
            if(x >= TEXTURE_WIDTH)
                break;
                
        }

        x = rg[_x] % TEXTURE_WIDTH;
        y++;

        if(y >= TEXTURE_HEIGHT)
            break;
    }
    
    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Display::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::togglePixel(uint32_t *px) {
    if(*px == 0xFFFFFFFF) {
        *px = 0x000000FF;
        return;
    }

    *px = 0xFFFFFFFF;
}