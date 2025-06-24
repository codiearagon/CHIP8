#include <cstdint>
#include <array>

class Memory {
    public:
        Memory();
        ~Memory();

        uint8_t read(uint16_t addr) const;
        void write(uint16_t addr, uint8_t value);

    private:
        std::array<uint8_t, 4096> memory;
 };