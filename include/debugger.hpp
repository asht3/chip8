#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

class Debugger {
    public:
        // Function to format hex values
        static std::string hex8(uint8_t value) {
            std::stringstream ss;
            ss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)value;
            return ss.str();
        }

        static std::string hex16(uint16_t value) {
            std::stringstream ss;
            ss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << value;
            return ss.str();
        }

        static void print_debug(uint16_t pc, uint16_t opcode, uint16_t I, uint8_t V[16], 
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
        static std::string get_mnemonic_simple(uint16_t opcode) {
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