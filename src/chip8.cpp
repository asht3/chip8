#include "../include/chip8.hpp"
#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include "../include/display.hpp"
#include "../include/input.hpp"

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

// void Chip8::run();

// void Chip8::stop();

// Display& get_display();

bool Chip8::is_running() {
    return running;
}