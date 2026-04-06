#include "../include/chip8.hpp"
#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include "../include/display.hpp"
#include "../include/input.hpp"
#include <exception>
#include <iostream> // For debugging output
#include <chrono>
#include <thread>

Chip8::Chip8() {
    reset();
}

void Chip8::reset() {
    cpu.reset();
    memory.reset();
    display.clear();
    input.reset();
    running = false;
}

void Chip8::emulate_cycle() {
    if (cpu.is_waiting_for_key()) {
        cpu.update_timers();
        return;
    }
    cpu.execute_cycle(memory, display, input);
    cpu.update_timers();
}

void Chip8::run() {
    running = true;
    SDL_Event event;
    
    // Frame-based loop at 60 FPS
    const int TARGET_FPS = 60;
    const int CYCLES_PER_FRAME = 15;
    const auto FRAME_DURATION = std::chrono::nanoseconds(1000000000 / TARGET_FPS);  // 16,666,667 ns = 16,666.66... ms
    
    auto next_frame_time = std::chrono::steady_clock::now();
    
    while (running) {
        // Set vblank ready at start of each frame
        display.set_vblank();
        
        // Process input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
                break;
            } else {
                input.handle_input_sdl(event, input);
                
                // Handle key waiting state
                if (event.type == SDL_KEYDOWN && cpu.is_waiting_for_key()) {
                    uint8_t chip8_key = 0xFF;
                    switch (event.key.keysym.sym) {
                        case SDLK_1: chip8_key = 0x1; break;
                        case SDLK_2: chip8_key = 0x2; break;
                        case SDLK_3: chip8_key = 0x3; break;
                        case SDLK_4: chip8_key = 0xC; break;
                        case SDLK_q: chip8_key = 0x4; break;
                        case SDLK_w: chip8_key = 0x5; break;
                        case SDLK_e: chip8_key = 0x6; break;
                        case SDLK_r: chip8_key = 0xD; break;
                        case SDLK_a: chip8_key = 0x7; break;
                        case SDLK_s: chip8_key = 0x8; break;
                        case SDLK_d: chip8_key = 0x9; break;
                        case SDLK_f: chip8_key = 0xE; break;
                        case SDLK_z: chip8_key = 0xA; break;
                        case SDLK_x: chip8_key = 0x0; break;
                        case SDLK_c: chip8_key = 0xB; break;
                        case SDLK_v: chip8_key = 0xF; break;
                    }
                    
                    if (chip8_key != 0xFF) {
                        cpu.handle_key_press(chip8_key);
                    }
                }
            }
        }
        
        if (!running) break;

        // Stop early if a draw blocked the frame
        for (int cycle = 0; cycle < CYCLES_PER_FRAME; cycle++) {
            if (cpu.is_waiting_for_key()) {
                break;
            } else {
                cpu.execute_cycle(memory, display, input);
                if (display.is_frame_blocked()) {
                    break;
                }
            }
        }
        
        // Update timers once per frame
        cpu.update_timers();
        
        if (cpu.get_sound_timer() > 0) {
            sound->beep();
        } else {
            sound->stop();
        }
        
        if (display.needs_redraw()) {
            display.render_to_sdl(10);
            display.clear_redraw_flag();
        }
        
        // Sleep until next tick
        next_frame_time += FRAME_DURATION;
        std::this_thread::sleep_until(next_frame_time);
    }
}

void Chip8::stop() {
    running = false;
}

void Chip8::load_rom(const char* filename) {
    try {
        cpu.reset();
        memory.load_rom(filename, cpu.START_ADDRESS);
        // for (int i = 0; i < 10; ++i) {
        //     printf("Opcode at %03X: %02X%02X\n", 0x200 + i*2, memory.read(0x200 + i*2), memory.read(0x200 + i*2 + 1));
        // }
        // DEBUG: Check what was loaded
        // std::cout << "ROM loaded, dumping first 32 bytes:\n";
        // memory.dump(0x200, 32);

        // return true;
    } catch (std::exception& e) {
        std::cerr << "Load ROM error: " << e.what() << std::endl;
        throw;
        // return false;
    }
}

Display& Chip8::get_display() {
    return display;
}

Input& Chip8::get_input() {
    return input;
}

bool Chip8::is_running() {
    return running;
}

CPU& Chip8::get_cpu() {
    return cpu;
}

void Chip8::set_sound(Sound* snd) {
    sound = snd;
}