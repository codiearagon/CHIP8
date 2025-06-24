#include <SDL2/SDL.h>

#define SCREEN_WIDTH 854
#define SCREEN_HEIGHT 480 

class Display {
    public:
        Display();
        ~Display();

        void clearDisplay();
        void tick();
        void destroy();

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;

        void mainLoop();
};