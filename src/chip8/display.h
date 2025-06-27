#include <SDL2/SDL.h>
#include "memory.h"

#define SCREEN_WIDTH 854
#define SCREEN_HEIGHT 480 

class Display {
    public:
        Display(Memory* _mem);
        ~Display();

        void clearDisplay();
        void draw(uint8_t x, uint8_t y, uint8_t n, uint16_t _ir);
        void destroy();

    private:
        Memory *mem;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;

        void mainLoop();
};