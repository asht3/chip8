#include "../include/chip8.hpp"
#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include "../include/display.hpp"
#include "../include/input.hpp"

Chip8::Chip8() {
    cpu.reset();
    // TODO: clear memory
    display.clear();
    // TODO: clear input
    running = false;
    // TODO: Load fontset into memory

}

// void Chip8::load_rom(const char* filename);
void Chip8::emulate_cycle() {
    uint16_t pc = cpu.get_pc();
    uint16_t opcode = memory.read(pc) << 8 | memory.read(pc + 1);

    cpu.execute_cycle(memory.get_memory(), display.get_pixels(), input.get_keys());
    cpu.update_timers();
}
// void Chip8::run();
// void Chip8::stop();

// Display& get_display();
// bool Chip8::is_running();