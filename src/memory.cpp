#include "../include/memory.hpp"
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream> // For debugging output
#include <iomanip>  // For std::setw and std::setfill
#include <cstring>

Memory::Memory() {
    reset();
}

void Memory::reset() {
    // Initialize memory to zero
    std::fill(std::begin(memory), std::end(memory), 0);
    load_fontset();
}

void Memory::dump(uint16_t start, uint16_t length) const {
    std::cout << "Memory dump from 0x" << std::hex << start << ":\n";
    for (uint16_t i = start; i < start + length; i++) {
        if ((i - start) % 16 == 0) {
            std::cout << "\n0x" << std::hex << i << ": ";
        }
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(memory[i]) << " ";
    }
    std::cout << std::endl;
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

void Memory::load_rom(const std::string& filename, uint16_t start_address) {
    std::cout << "Opening: " << filename << std::endl;

    // Open ROM file in binary mode
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open ROM file"); 
    }

    // Get file size
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::cout << "File size: " << file_size << " bytes" << std::endl;
    std::cout << "Loading at address: 0x" << std::hex << start_address << std::endl;

    // Check if ROM fits in memory
    if (start_address + file_size > 4096) {
        throw std::runtime_error("ROM size exceeds memory bounds");
    }

    // Read file contents into memory starting at start_address
    file.read(reinterpret_cast<char*>(&memory[start_address]), file_size);
    if (!file) {
        std::cout << "Read failed! Error: " << strerror(errno) << std::endl;
        std::cout << "Bytes read: " << file.gcount() << std::endl;
        throw std::runtime_error("Failed to read ROM file");
    }

    std::cout << "Successfully read " << file_size << " bytes" << std::endl;
    
    // Verify by dumping what was read
    std::cout << "First 16 bytes loaded: ";
    for (int i = 0; i < 16; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(memory[start_address + i]) << " ";
    }
    std::cout << std::endl;
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