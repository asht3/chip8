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
    chip8.get_display().init_sdl("CHIP-8", 10);

    // Start emulation
    chip8.run();
    SDL_Event event;
    while (chip8.is_running()) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                chip8.stop();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                chip8.stop();
            } else {
                chip8.get_input().handle_input_sdl(event, chip8.get_input());
            }
        }
  
        chip8.emulate_cycle();
        if (chip8.get_display().needs_redraw()) {
            chip8.get_display().render_to_sdl(10);
            chip8.get_display().clear_redraw_flag();
            // Small delay to see the display
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }
    chip8.get_display().cleanup_sdl();
    return 0;
}