#include <vector>
#include <stack>
#include "memory.h"

class Chip8 {
    public:
        Chip8();
        ~Chip8();
        
        void loadRom(std::string fileName);
        uint16_t fetch();
        void decode();
        void run();
        
    private:
        Memory *memory;
        std::stack<uint16_t> stack;
        uint8_t registers[16];

        uint16_t pc;
        uint16_t ir;
};