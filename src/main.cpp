#include "../include/chip8.hpp"
#include "../include/display.hpp"
#include "../include/input.hpp"
#include <thread>
#include <iostream> // Debug

int main() {
    // Load Chip8 ROM
    Chip8 chip8;
    // chip8.load_rom("./roms/2-ibm-logo.ch8");
    chip8.load_rom("./roms/6-keypad.ch8");

    // Start emulation
    chip8.run();
    while (chip8.is_running()) {
        // chip8.get_input().process_terminal_input();
        //  if (chip8.get_input().is_waiting_for_key()) {           
        //     while (chip8.get_input().is_waiting_for_key()) {
        //         chip8.get_input().process_terminal_input();
        //         std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //     }
        // }
        
        chip8.emulate_cycle();
        if (chip8.get_display().needs_redraw()) {
            chip8.get_display().render_to_console();
            chip8.get_display().clear_redraw_flag();
            
            // Small delay to see the display
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

        // static int cycle_count = 0;
        // if (cycle_count++ > 10000) chip8.stop();
    }
    // TODO: Add a way to exit program
    return 0;
}