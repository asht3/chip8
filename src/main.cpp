#include "../include/chip8.hpp"
#include "../include/display.hpp"
#include <thread>
#include <iostream> // Debug
#include "../include/memory.hpp"
#include <iomanip>

int main() {
    // Load Chip8 ROM
    Chip8 chip8;
    chip8.load_rom("./roms/2-ibm-logo.ch8");
    
//     Memory& memory = chip8.memory; // Access memory for debugging
// // After loading the ROM, dump the sprite data areas
// std::cout << "Sprite data at 0x22a (left side):" << std::endl;
// for (int i = 0; i < 16; i++) {
//     std::cout << std::hex << std::setw(2) << std::setfill('0') 
//               << static_cast<int>(memory.read(0x22a + i)) << " ";
// }
// std::cout << std::endl;

// std::cout << "Sprite data at 0x248 (right side):" << std::endl;
// for (int i = 0; i < 16; i++) {
//     std::cout << std::hex << std::setw(2) << std::setfill('0') 
//               << static_cast<int>(memory.read(0x248 + i)) << " ";
// }
// std::cout << std::endl;

    // Start emulation
    // chip8.run();
    // while (chip8.is_running()) {
    //     chip8.emulate_cycle();
    //     if (chip8.get_display().needs_redraw()) {
    //         chip8.get_display().render_to_console();
    //         chip8.get_display().clear_redraw_flag();
            
    //         // Small delay to see the display
    //         std::this_thread::sleep_for(std::chrono::milliseconds(2));
    //     }

    //     static int cycle_count = 0;
    //     if (cycle_count++ > 10000) chip8.stop();
    // }
    // TODO: Add a way to exit program

     // Let the logo draw completely first
    for (int i = 0; i < 50; i++) {  // Execute enough cycles to draw the logo
        chip8.emulate_cycle();
    }
    
    // Now render and wait
   chip8.get_display().render_to_console();
    std::cout << "IBM Logo Complete!" << std::endl;
    return 0;
}