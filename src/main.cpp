#include "../include/chip8.hpp"
#include "../include/display.hpp"
#include "../include/input.hpp"
#include "../include/sound.hpp"
// debug
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    // Load Chip8 ROM
    Chip8 chip8;
    chip8.load_rom("./roms/br8kout.ch8");
    // chip8.load_rom("./roms/Tetris [Fran Dachille, 1991].ch8");
    // chip8.load_rom("./roms/Space Invaders [David Winter].ch8");
    // chip8.load_rom("./roms/Pong (1 player).ch8");

    // chip8.load_rom("./roms/4-flags.ch8");
    // chip8.load_rom("./roms/5-quirks.ch8");
    // chip8.load_rom("./roms/3-corax+.ch8");
    // chip8.load_rom("./roms/6-keypad.ch8");

    chip8.get_display().init_sdl("CHIP-8", 10);
    
    Sound sound;
    sound.init();
    chip8.set_sound(&sound);

    // std::cout << "Starting emulation..." << std::endl;
    // auto start = std::chrono::steady_clock::now();

    chip8.run();

    // auto end_run = std::chrono::steady_clock::now();
    // std::cout << "run() completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end_run - start).count() << "ms" << std::endl;

    // std::cout << "Starting sound cleanup..." << std::endl;

    sound.cleanup();

    // auto end_sound = std::chrono::steady_clock::now();
    // std::cout << "sound.cleanup() completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end_sound - end_run).count() << "ms" << std::endl;

    // std::cout << "Starting display cleanup..." << std::endl;

    chip8.get_display().cleanup_sdl();

    // auto end_display = std::chrono::steady_clock::now();
    // std::cout << "cleanup_sdl() completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end_display - end_sound).count() << "ms" << std::endl;

    // std::cout << "Program exiting..." << std::endl;
    return 0;
}