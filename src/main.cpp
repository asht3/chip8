#include "../include/chip8.hpp"

int main() {
    // Load Chip8 ROM
    Chip8 chip8;
    chip8.load_rom("./roms/3-corax+.ch8");
    
    // Start emulation
    chip8.run();
    while (chip8.is_running()) {
        chip8.emulate_cycle();
    }
    return 0;
}