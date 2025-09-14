#include "../include/cpu.hpp"
#include <algorithm>
// #include <cstdint>
#include <stdexcept>

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
    if (SP == 0) {
        throw std::runtime_error("Stack underflow on RET");
    }
    PC = stack[--SP];
}

void CPU::OP_1nnn(uint16_t opcode) {
    PC = opcode & 0x0FFF;
}

void CPU::OP_2nnn(uint16_t opcode) {
    if (SP >= 16) {
        throw std::runtime_error("Stack overflow on CALL");
    }
    stack[SP++] = PC;
    PC = opcode & 0x0FFF;
}