#include "memory.h"

class Chip8 {
    public:
        Chip8();
        ~Chip8();
        
        void run();
        
    private:
        Memory *memory;
};