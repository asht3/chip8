#pragma once
#include <cstdint>

class Memory {
    private:
        uint8_t memory[4096]; // 4K memory

    public:
        Memory();
        uint8_t* get_memory();
        uint8_t read(uint16_t address) const;
        void write(uint16_t address, uint8_t value);
        void load_rom(const char* filename, uint16_t start_address);
        void load_fontset();
};