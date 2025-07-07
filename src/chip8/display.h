#include <SDL2/SDL.h>
#include "memory.h"

#define SCREEN_WIDTH 854
#define SCREEN_HEIGHT 480 
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 32

class Display {
    public:
        Display(Memory* _mem);
        ~Display();

        void clearDisplay();
        void draw(uint8_t *rg, uint8_t _x, uint8_t _y, uint8_t n, uint16_t& _ir);
        void destroy();

    private:
        Memory *mem;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;

        void mainLoop();
        void togglePixel(uint32_t *px);
};