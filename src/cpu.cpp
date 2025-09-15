#include "../include/cpu.hpp"
#include <algorithm>
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

void CPU::OP_3xkk(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    if (V[Vx] == kk) {
        PC += 2;
    }
}

void CPU::OP_4xkk(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    if (V[Vx] != kk) {
        PC += 2;
    }
}

void CPU::OP_5xy0(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    if (V[Vx] == V[Vy]) {
        PC += 2;
    }
}

void CPU::OP_6xkk(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    V[Vx] = kk;
}

void CPU::OP_7xkk(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    V[Vx] += kk;
}

void CPU::OP_8xy0(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    V[Vx] = Vy;
}

void CPU::OP_8xy1(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    // V[Vx] |= Vy;
    V[Vx] = V[Vx] | V[Vy];
}

void CPU::OP_8xy2(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    // V[Vx] &= Vy;
    V[Vx] = V[Vx] & V[Vy];
}

void CPU::OP_8xy3(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    V[Vx] = V[Vx] ^ V[Vy];
}

void CPU::OP_8xy4(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    uint16_t sum = V[Vx] + V[Vy];
    V[0xF] = (sum > 0xFF) ? 1 : 0;
    V[Vx] = sum & 0xFF;
    // Vx = V[Vx] + V[Vy];
    // V[0xF] = (Vx > 0xFF) ? 1 : 0; // Set carry flag
    // V[Vx] = Vx & 0xFF; // Keep only the lower 8 bits
}

void CPU::OP_8xy5(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    V[0xF] = (V[Vx] > V[Vy]) ? 1 : 0;
    V[Vx] -= V[Vy];
}

void CPU::OP_8xy6(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    V[0xF] = V[Vx] & 0x1; // Store LSB of Vx in VF
    V[Vx] >>= 1; // Shift right by 1 (divide by 2)
}

void CPU::OP_8xy7(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    V[Vx] = V[Vy] - V[Vx];
    V[0xF] = (V[Vy] > V[Vx]) ? 1 : 0;
}

void CPU::OP_8xyE(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    V[0xF] = (V[Vx] & 0x80) >> 7; // Store MSB of Vx in VF
    V[Vx] <<= 1; // Shift left by 1 (multiply by 2)
}

void CPU::OP_9xy0(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    if (V[Vx] != V[Vy]) {
        PC += 2;
    }
}

void CPU::OP_Annn(uint16_t opcode) {
    I = opcode & 0x0FFF;
    PC += 2;
}

void CPU::OP_Bnnn(uint16_t opcode) {
    uint16_t nnn = opcode & 0x0FFF;
    PC = V[0] + nnn;
}

void CPU::OP_Cxkk(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    V[Vx] = (rand() % 256) & kk;
}

void CPU::OP_Dxyn(uint8_t* memory, uint8_t* display, uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    uint8_t height = opcode & 0x000F;
    // Coordinates
    uint8_t x = V[Vx] % 64;
    uint8_t y = V[Vy] % 32;
    V[0xF] = 0;

    for (int row = 0; row < height; row++) {
        uint8_t sprite_byte = memory[I + row];
        for (int col = 0; col < 8; col++) {
            if ((sprite_byte & (0x80 >> col)) != 0) { // Check if the current bit is set
                int display_index = (x + col) % 64 + ((y + row) % 32) * 64;
                if (display[display_index] == 1) {
                    V[0xF] = 1; // Collision detected
                }
                display[display_index] ^= 1; // XOR the pixel
            }
        }
    }
}

void CPU::OP_Ex9E(uint16_t opcode, bool* keys) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    // Check if key is valid and pressed
    if (V[Vx] < 16 && keys[V[Vx]]) {
        PC += 2;
    }
}

void CPU::OP_ExA1(uint16_t opcode, bool* keys) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    // Check if key is valid and not pressed
    if (V[Vx] < 16 && !keys[V[Vx]]) {
        PC += 2;
    }
}

void CPU::OP_Fx07(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    V[Vx] = delay_timer;
}

void CPU::OP_Fx0A(uint16_t opcode, bool* keys) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i < 16; i++) {
        if (keys[i]) {
            V[Vx] = i;
            return;
        }
    }
    // If no key is pressed, do not advance the PC
    PC -= 2; // Stay on the same instruction
}

void CPU::OP_Fx15(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    delay_timer = V[Vx];
}

void CPU::OP_Fx18(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    sound_timer = V[Vx];
}

void CPU::OP_Fx1E(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    I = (I + V[Vx]) & 0x0FFF; // I stays within 12 bits
}

void CPU::OP_Fx29(uint16_t opcode) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t character = V[Vx] & 0x0F; // Only 0-F are valid characters
    I = character * 5; // Each character is 5 bytes
}

void CPU::OP_Fx33(uint16_t opcode, uint8_t* memory) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    memory[I] = V[Vx] / 100;
    memory[I + 1] = V[Vx] / 10 % 10;
    memory[I + 2] = V[Vx] % 10;
}

void CPU::OP_Fx55(uint16_t opcode, uint8_t* memory) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i <= Vx; i++) {
        memory[I + i] = V[i];
    }
    I += Vx + 1;
}

void CPU::OP_Fx65(uint16_t opcode, uint8_t* memory) {
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i <= Vx; i++) {
        V[i] = memory[I + i];
    }
    I += Vx + 1;
}