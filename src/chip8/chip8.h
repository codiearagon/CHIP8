#include <vector>
#include <stack>
#include <random>
#include "audio.h"
#include "display.h"

class Chip8 {
    public:
        enum Mode {ORIGINAL, MODERN};
        Chip8();
        ~Chip8();
        
        void loadRom(std::string fileName);
        void selectMode(Mode mode);
        uint16_t fetch();
        void decode();
        void run();
        
    private:
        Mode mode;
        Display *display;
        Audio *audio;
        Memory *memory;
        std::stack<uint16_t> stack;
        uint8_t registers[16] {0};
        uint8_t keypad[16] {0};

        uint8_t delayTimer = 0;
        uint8_t soundTimer = 0;
        uint32_t lastTime = 0;

        uint16_t pc;
        uint16_t ir;

        std::random_device rd;
        std::mt19937 gen;
        std::uniform_int_distribution<uint8_t> dist;

        void handleEvents(SDL_Event& event, bool& running);
        void updateTimers();
};