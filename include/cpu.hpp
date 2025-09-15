#pragma once
#include <cstdint>

class CPU {
    private:
        uint8_t V[16];        // Registers V0 to VF
        uint16_t I;           // Index register
        uint16_t PC;          // Program counter
        uint8_t delay_timer;  // Delay timer
        uint8_t sound_timer;  // Sound timer
        uint16_t stack[16];   // Stack for subroutine calls
        uint8_t SP;           // Stack pointer

        uint16_t fetch_opcode(uint8_t* memory);
        void update_timers();

        // Opcode handlers
        void OP_00E0(uint8_t* display);                   // Clear screen
        void OP_00EE();                                   // Return from subroutine
        void OP_1nnn(uint16_t opcode);                    // Jump to address
        void OP_2nnn(uint16_t opcode);                    // Call subroutine
        void OP_3xkk(uint16_t opcode);                    // Skip if Vx == kk
        void OP_4xkk(uint16_t opcode);                    // Skip if Vx != kk
        void OP_5xy0(uint16_t opcode);                    // Skip if Vx == Vy
        void OP_6xkk(uint16_t opcode);                    // Set Vx = kk
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
        void OP_9xy0(uint16_t opcode);                    // Skip if Vx != Vy
        void OP_Annn(uint16_t opcode);                    // Set I = nnn
        void OP_Bnnn(uint16_t opcode);                    // Jump to V0 + nnn
        void OP_Cxkk(uint16_t opcode);                    // Set Vx = random byte AND kk
        void OP_Dxyn(uint8_t* memory, uint8_t* display, uint16_t opcode);  // Draw sprite
        void OP_Ex9E(uint16_t opcode, bool* keys);        // Skip if key pressed
        void OP_ExA1(uint16_t opcode, bool* keys);        // Skip if key not pressed
        void OP_Fx07(uint16_t opcode);                    // Set Vx = delay timer
        void OP_Fx0A(uint16_t opcode, bool* keys);        // Wait for key press
        void OP_Fx15(uint16_t opcode);                    // Set delay timer = Vx
        void OP_Fx18(uint16_t opcode);                    // Set sound timer = Vx
        void OP_Fx1E(uint16_t opcode);                    // Set I = I + Vx
        void OP_Fx29(uint16_t opcode);                    // Set I to sprite address for character in Vx
        void OP_Fx33(uint16_t opcode, uint8_t* memory);   // Store BCD representation of Vx in memory
        void OP_Fx55(uint16_t opcode, uint8_t* memory);   // Store registers V0 through Vx in memory
        void OP_Fx65(uint16_t opcode, uint8_t* memory);   // Read registers V0 through Vx from memory

    public:
        CPU();
        void reset();
        void execute_cycle(uint8_t* memory, uint8_t* display, bool* keys);
        // void set_delay_timer(uint8_t value);
        // uint8_t get_delay_timer() const;
        // void set_sound_timer(uint8_t value);
        // uint8_t get_sound_timer() const;
        // uint16_t get_pc() const;
        // void set_pc(uint16_t address);
        // uint16_t get_I() const;
        // void set_I(uint16_t address);
        // uint8_t get_V(uint8_t index) const;
        // void set_V(uint8_t index, uint8_t value);
        void push_stack(uint16_t address);
        uint16_t pop_stack();
};