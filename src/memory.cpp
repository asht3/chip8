#include "../include/memory.hpp"
#include <algorithm>
#include <stdexcept>

Memory::Memory() {
    reset();
}

void Memory::reset() {
    // Initialize memory to zero
    std::fill(std::begin(memory), std::end(memory), 0);
    load_fontset();
}

uint8_t* Memory::get_memory() {
    return memory;
}

uint8_t Memory::read(uint16_t address) const {
    if (address >= 4096) {
        throw std::out_of_range("Memory read out of bounds");
    }
    return memory[address];
}

void Memory::write(uint16_t address, uint8_t value) {
    if (address >= 4096) {
        throw std::out_of_range("Memory write out of bounds");
    }
    memory[address] = value;
}

void Memory::load_rom(const char* filename, uint16_t start_address) {
    // Open ROM file in binary mode
    FILE* file = fopen(filename, "rb");
    if (!file) {
        throw std::runtime_error("Failed to open ROM file");
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Check if ROM fits in memory
    if (start_address + file_size > 4096) {
        fclose(file);
        throw std::runtime_error("ROM size exceeds memory bounds");
    }

    // Read file contents into memory starting at start_address
    ssize_t read_size = fread(&memory[start_address], 1, file_size, file);
    if (read_size != file_size) {
        fclose(file);
        throw std::runtime_error("Failed to read ROM file");
    }
    fclose(file);
}

void Memory::load_fontset() {
    uint8_t fontset[FONTSET_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Start loading fontset at memory location 0x50 (80)
    for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
        memory[FONTSET_SIZE + i] = fontset[i];
    }
    // for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
    //     memory[i] = fontset[i];
    // }
}