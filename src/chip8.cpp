#include "../include/chip8.hpp"
#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include "../include/display.hpp"
#include "../include/input.hpp"
#include <exception>
#include <iostream> // For debugging output

Chip8::Chip8() {
    reset();
}

void Chip8::reset() {
    cpu.reset();
    memory.reset();
    display.clear();
    input.reset();
    running = false;
}

void Chip8::emulate_cycle() {
    cpu.execute_cycle(memory, display, input);
    cpu.update_timers();
}

void Chip8::run() {
    running = true;
    emulate_cycle();
}

void Chip8::stop() {
    running = false;
}

void Chip8::load_rom(const char* filename) {
    try {
        memory.load_rom(filename, cpu.START_ADDRESS);

        // DEBUG: Check what was loaded
        std::cout << "ROM loaded, dumping first 32 bytes:\n";
        memory.dump(0x200, 32);

        cpu.reset();
        // return true;
    } catch (std::exception& e) {
        std::cerr << "Load ROM error: " << e.what() << std::endl;
        throw;
        // return false;
    }
}

Display& Chip8::get_display() {
    return display;
}

bool Chip8::is_running() {
    return running;
}