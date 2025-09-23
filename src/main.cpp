#include "../include/chip8.hpp"

int main() {
    // Load Chip8 ROM
    Chip8 chip8;
    chip8.load_rom("./roms/2-ibm-logo.ch8");
    
    // Start emulation
    // fetch display and render loop here
    int i = 0;
    chip8.run();
    while (chip8.is_running()) {
        chip8.emulate_cycle();
        chip8.get_display(); // TODO: render display

        if (i == 30) chip8.stop();
        else i++; // note if you remove this, it will be an infinite loop at PC: 0x49e Opcode: 0x149c for corax
    }
    return 0;
}