#pragma once
#include <cstdint>
#include "../include/memory.hpp"
#include "../include/display.hpp"
#include "../include/input.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
class CPU {
    private:
        uint8_t V[16];        // Registers V0 to VF
        uint16_t I;           // Index register
        uint16_t PC;          // Program counter
        uint8_t delay_timer;  // Delay timer
        uint8_t sound_timer;  // Sound timer
        uint16_t stack[16];   // Stack for subroutine calls
        uint8_t SP;           // Stack pointer
        bool waiting_for_key; // Flag for waiting for key press
        uint8_t key_register;  // Register to store the key press

        uint16_t fetch_opcode(Memory& memory);

        // Opcode handlers
        // Program flow
        void OP_00E0(Display& display);                   // Clear screen
        void OP_00EE();                                   // Return from subroutine
        void OP_1nnn(uint16_t opcode);                    // Jump to address
        void OP_2nnn(uint16_t opcode);                    // Call subroutine
        void OP_Bnnn(uint16_t opcode);                    // Jump to V0 + nnn

        // Conditional skips
        void OP_3xkk(uint16_t opcode);                    // Skip if Vx == kk
        void OP_4xkk(uint16_t opcode);                    // Skip if Vx != kk
        void OP_9xy0(uint16_t opcode);                    // Skip if Vx != Vy
        void OP_Ex9E(uint16_t opcode, Input& keys);        // Skip if key pressed
        void OP_ExA1(uint16_t opcode, Input& keys);        // Skip if key not pressed

        void OP_5xy0(uint16_t opcode);                    // Skip if Vx == Vy

        // Arithmetic and movement logic
        void OP_7xkk(uint16_t opcode);                    // Add kk to Vx
        void OP_8xy0(uint16_t opcode);                    // Set Vx = Vy
        void OP_8xy1(uint16_t opcode);                    // Set Vx = Vx OR Vy
        void OP_8xy2(uint16_t opcode);                    // Set Vx = Vx AND Vy
        void OP_8xy3(uint16_t opcode);                    // Set Vx = Vx XOR Vy
        void OP_8xy4(uint16_t opcode);                    // Add Vy to Vx (with carry)
        void OP_8xy5(uint16_t opcode);                    // Subtract Vy from Vx
        void OP_8xy6(uint16_t opcode);                    // Shift Vx right
        void OP_8xy7(uint16_t opcode);                    // Set Vx = Vy - Vx
        void OP_8xyE(uint16_t opcode);                    // Shift Vx left

        void OP_Cxkk(uint16_t opcode);                    // Set Vx = random byte AND kk
        void OP_Dxyn(Memory& memory, Display& display, uint16_t opcode);  // Draw sprite
        
        // Timer and sound
        void OP_Fx07(uint16_t opcode);                    // Set Vx = delay timer
        void OP_Fx0A(uint16_t opcode, Input& keys);        // Wait for key press
        void OP_Fx15(uint16_t opcode);                    // Set delay timer = Vx
        void OP_Fx18(uint16_t opcode);                    // Set sound timer = Vx
        void OP_Fx1E(uint16_t opcode);                    // Set I = I + Vx
        void OP_Fx29(uint16_t opcode);                    // Set I to sprite address for character in Vx
        void OP_Fx33(uint16_t opcode, Memory& memory);   // Store BCD representation of Vx in memory

        // Memory management
        void OP_6xkk(uint16_t opcode);                    // Set Vx = kk
        void OP_Annn(uint16_t opcode);                    // Set I = nnn
        void OP_Fx55(uint16_t opcode, Memory& memory);   // Store registers V0 through Vx in memory
        void OP_Fx65(uint16_t opcode, Memory& memory);   // Read registers V0 through Vx from memory

    public:
        const unsigned int START_ADDRESS = 0x200;

        CPU();
        void reset();
        void execute_cycle(Memory& memory, Display& display, Input& keys);
        void update_timers();
        uint8_t get_sound_timer() const;
        uint16_t get_pc() const;
        void set_pc(uint16_t address);
        void add_to_pc(uint16_t offset);
        void set_V(uint8_t index, uint8_t value);
        void push_stack(uint16_t address);
        uint16_t pop_stack();
        bool is_waiting_for_key() const;
        void handle_key_press(uint8_t key);

        // Function to format hex values
        inline std::string hex8(uint8_t value) {
            std::stringstream ss;
            ss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)value;
            return ss.str();
        }

        inline std::string hex16(uint16_t value) {
            std::stringstream ss;
            ss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << value;
            return ss.str();
        }

        inline void print_debug(uint16_t pc, uint16_t opcode, uint16_t I, uint8_t V[16], 
                                const std::string& last_mnemonic, const std::string& current_mnemonic) {
            
            // Clear the last debug output (move cursor up 12 lines)
            std::cout << "\033[12A";
            
            // Print header
            std::cout << "\033[36m════════════════════════════════════════════════════════════════════\033[0m\n";
            std::cout << "\033[33mCHIP-8 DEBUGGER\033[0m\n";
            std::cout << "\033[36m════════════════════════════════════════════════════════════════════\033[0m\n\n";
            
            std::cout << "\033[32mProgram Counter:\033[0m " << hex16(pc) << "\n";
            std::cout << "\033[32mI Register:\033[0m      " << hex16(I) << "\n\n";
            std::cout << "\033[33mLast Instruction:\033[0m  " << last_mnemonic << "\n";
            std::cout << "\033[33mCurrent Instruction:\033[0m " << current_mnemonic << " \033[90m(Opcode: " << hex16(opcode) << ")\033[0m\n\n";
            
            // V Registers
            std::cout << "\033[32mV Registers:\033[0m\n";
            for (int i = 0; i < 16; i += 4) {
                std::cout << "\033[36m│\033[0m ";
                for (int j = 0; j < 4; j++) {
                    int reg = i + j;
                    std::cout << "V" << std::hex << std::uppercase << reg << "=" << hex8(V[reg]);
                    if (j < 3) std::cout << " \033[36m│\033[0m ";
                }
                std::cout << " \033[36m│\033[0m\n";
            }
            std::cout << "\033[36m════════════════════════════════════════════════════════════════════\033[0m\n";
            
            // Small delay to prevent flickering
            std::cout.flush();
        }

        // Function to get instruction mnemonic
        inline std::string get_mnemonic_simple(uint16_t opcode) {
            uint16_t masked = opcode & 0xF000;
            
            switch (masked) {
                case 0x0000:
                    if (opcode == 0x00E0) return "CLS";
                    if (opcode == 0x00EE) return "RET";
                    return "???";
                case 0x1000: return "JP " + hex16(opcode & 0x0FFF);
                case 0x2000: return "CALL " + hex16(opcode & 0x0FFF);
                case 0x3000: return "SE V" + std::to_string((opcode & 0x0F00) >> 8) + ", " + hex8(opcode & 0x00FF);
                case 0x4000: return "SNE V" + std::to_string((opcode & 0x0F00) >> 8) + ", " + hex8(opcode & 0x00FF);
                case 0x5000: return "SE V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4);
                case 0x6000: return "LD V" + std::to_string((opcode & 0x0F00) >> 8) + ", " + hex8(opcode & 0x00FF);
                case 0x7000: return "ADD V" + std::to_string((opcode & 0x0F00) >> 8) + ", " + hex8(opcode & 0x00FF);
                case 0x8000:
                    switch (opcode & 0x000F) {
                        case 0x0: return "LD V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4);
                        case 0x1: return "OR V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4);
                        case 0x2: return "AND V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4);
                        case 0x3: return "XOR V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4);
                        case 0x4: return "ADD V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4);
                        case 0x5: return "SUB V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4);
                        case 0x6: return "SHR V" + std::to_string((opcode & 0x0F00) >> 8);
                        case 0x7: return "SUBN V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4);
                        case 0xE: return "SHL V" + std::to_string((opcode & 0x0F00) >> 8);
                        default: return "???";
                    }
                case 0x9000: return "SNE V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4);
                case 0xA000: return "LD I, " + hex16(opcode & 0x0FFF);
                case 0xB000: return "JP V0, " + hex16(opcode & 0x0FFF);
                case 0xC000: return "RND V" + std::to_string((opcode & 0x0F00) >> 8) + ", " + hex8(opcode & 0x00FF);
                case 0xD000: return "DRW V" + std::to_string((opcode & 0x0F00) >> 8) + ", V" + std::to_string((opcode & 0x00F0) >> 4) + ", " + std::to_string(opcode & 0x000F);
                case 0xE000:
                    if ((opcode & 0x00FF) == 0x009E) return "SKP V" + std::to_string((opcode & 0x0F00) >> 8);
                    if ((opcode & 0x00FF) == 0x00A1) return "SKNP V" + std::to_string((opcode & 0x0F00) >> 8);
                    return "???";
                case 0xF000:
                    switch (opcode & 0x00FF) {
                        case 0x07: return "LD V" + std::to_string((opcode & 0x0F00) >> 8) + ", DT";
                        case 0x0A: return "LD V" + std::to_string((opcode & 0x0F00) >> 8) + ", K";
                        case 0x15: return "LD DT, V" + std::to_string((opcode & 0x0F00) >> 8);
                        case 0x18: return "LD ST, V" + std::to_string((opcode & 0x0F00) >> 8);
                        case 0x1E: return "ADD I, V" + std::to_string((opcode & 0x0F00) >> 8);
                        case 0x29: return "LD F, V" + std::to_string((opcode & 0x0F00) >> 8);
                        case 0x33: return "LD B, V" + std::to_string((opcode & 0x0F00) >> 8);
                        case 0x55: return "LD [I], V" + std::to_string((opcode & 0x0F00) >> 8);
                        case 0x65: return "LD V" + std::to_string((opcode & 0x0F00) >> 8) + ", [I]";
                        default: return "???";
                    }
                default: return "???";
            }
        }
};