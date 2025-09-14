#include "cpu.hpp"
#include <algorithm>
#include <cstdint>

CPU::CPU() {
    reset();
}

void CPU::reset() {
    std::fill(std::begin(V), std::end(V), 0);
    I = 0;
    PC = 0x200; // Start at memory location 0x200
    delay_timer = 0;
    sound_timer = 0;
    std::fill(std::begin(stack), std::end(stack), 0);
    SP = 0;
}

uint16_t CPU::fetch_opcode(uint8_t* memory) {
    return (memory[PC] << 8) | memory[PC + 1];
}

void CPU::OP_00E0(uint8_t* display) {
    // Clear the display by setting all pixels to 0
    std::fill(display, display + 64 * 32, 0);
}

void CPU::OP_00EE() {
    PC = stack[--SP];
}