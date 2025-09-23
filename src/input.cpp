#include "../include/input.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream> // Debug
// #include <unistd.h>
// #include <termios.h>
// #include <fcntl.h>

Input::Input() {
    reset();  // Initialize with no keys pressed
}

void Input::reset() {
    std::fill(std::begin(keys), std::end(keys), false);
}

void Input::handle_input_sdl(Input& chip8_input) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                break;
                
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                bool pressed = (event.type == SDL_KEYDOWN);
                switch (event.key.keysym.sym) {
                    // Row 1: 1 2 3 4
                    case SDLK_1: chip8_input.set_key(0x1, pressed); break;
                    case SDLK_2: chip8_input.set_key(0x2, pressed); break;
                    case SDLK_3: chip8_input.set_key(0x3, pressed); break;
                    case SDLK_4: chip8_input.set_key(0xC, pressed); break;
                    
                    // Row 2: Q W E R
                    case SDLK_q: chip8_input.set_key(0x4, pressed); break;
                    case SDLK_w: chip8_input.set_key(0x5, pressed); break;
                    case SDLK_e: chip8_input.set_key(0x6, pressed); break;
                    case SDLK_r: chip8_input.set_key(0xD, pressed); break;
                    
                    // Row 3: A S D F
                    case SDLK_a: chip8_input.set_key(0x7, pressed); break;
                    case SDLK_s: chip8_input.set_key(0x8, pressed); break;
                    case SDLK_d: chip8_input.set_key(0x9, pressed); break;
                    case SDLK_f: chip8_input.set_key(0xE, pressed); break;
                    
                    // Row 4: Z X C V
                    case SDLK_z: chip8_input.set_key(0xA, pressed); break;
                    case SDLK_x: chip8_input.set_key(0x0, pressed); break;
                    case SDLK_c: chip8_input.set_key(0xB, pressed); break;
                    case SDLK_v: chip8_input.set_key(0xF, pressed); break;
                    
                    case SDLK_ESCAPE:
                        // Quit on escape
                        break;
                }
                break;
        }
    }
}

void Input::set_key(uint8_t key, bool pressed) {
    if (key < 16) {
        keys[key] = pressed;
    }
}

bool Input::is_pressed(int key) {
    if (key < 0 || key >= 16) {
        return false;
    }
    return keys[key];
}

uint8_t Input::wait_for_keypress() {
    for (uint8_t i = 0; i < 16; i++) {
        if (keys[i]) {
            return i;
        }
    }
    return 0xFF; // Indicate no key is pressed
}

// bool Input::is_waiting_for_key() const {
//     return waiting_for_key;
// }

// void Input::set_wait_state(uint8_t register_index) {
//     waiting_for_key = true;
//     wait_register = register_index;
//     // std::cout << "Waiting for key press for register V" << std::hex << (int)register_index << std::endl;
// }

// void Input::handle_key_press(uint8_t key) {
//     if (key < 16) {
//         keys[key] = true;
//         waiting_for_key = false;
//     }
// }

// uint8_t Input::get_wait_register() const {
//     return wait_register;
// }

// // Non-blocking keyboard input check (Linux/Mac)
// bool kbhit() {
//     struct termios oldt, newt;
//     int ch;
//     int oldf;

//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//     fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

//     ch = getchar();

//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     fcntl(STDIN_FILENO, F_SETFL, oldf);

//     if (ch != EOF) {
//         ungetc(ch, stdin);
//         return true;
//     }

//     return false;
// }

// void Input::process_terminal_input() {
//     std::fill(std::begin(keys), std::end(keys), false);
    
//     if (!kbhit()) return;
    
//     char key = getchar();
    
//     // Map keyboard keys to Chip-8 keypad
//     switch (key) {
//         // Original Chip-8 keypad layout:
//         // 1 2 3 C
//         // 4 5 6 D
//         // 7 8 9 E
//         // A 0 B F
        
//         // Modern mapping:
//         case '1': set_key(0x1, true); break;
//         case '2': set_key(0x2, true); break;
//         case '3': set_key(0x3, true); break;
//         case '4': set_key(0xC, true); break;
        
//         case 'q': case 'Q': set_key(0x4, true); break;
//         case 'w': case 'W': set_key(0x5, true); break;
//         case 'e': case 'E': set_key(0x6, true); break;
//         case 'r': case 'R': set_key(0xD, true); break;
        
//         case 'a': case 'A': set_key(0x7, true); break;
//         case 's': case 'S': set_key(0x8, true); break;
//         case 'd': case 'D': set_key(0x9, true); break;
//         case 'f': case 'F': set_key(0xE, true); break;
        
//         case 'z': case 'Z': set_key(0xA, true); break;
//         case 'x': case 'X': set_key(0x0, true); break;
//         case 'c': case 'C': set_key(0xB, true); break;
//         case 'v': case 'V': set_key(0xF, true); break;
        
//         case 27: // ESC
//             std::cout << "ESC pressed - exiting" << std::endl;
//             exit(0);
//             break;
            
//         default:
//             std::cout << "Unknown key: " << key << " (ASCII: " << (int)key << ")" << std::endl;
//             break;
//     }
    
//     if (waiting_for_key && key != 27) {
//         // Convert the key to Chip-8 key value
//         uint8_t chip8_key = 0;
//         switch (key) {
//             case '1': chip8_key = 0x1; break;
//             case '2': chip8_key = 0x2; break;
//             case '3': chip8_key = 0x3; break;
//             case '4': chip8_key = 0xC; break;
//             case 'q': case 'Q': chip8_key = 0x4; break;
//             case 'w': case 'W': chip8_key = 0x5; break;
//             case 'e': case 'E': chip8_key = 0x6; break;
//             case 'r': case 'R': chip8_key = 0xD; break;
//             case 'a': case 'A': chip8_key = 0x7; break;
//             case 's': case 'S': chip8_key = 0x8; break;
//             case 'd': case 'D': chip8_key = 0x9; break;
//             case 'f': case 'F': chip8_key = 0xE; break;
//             case 'z': case 'Z': chip8_key = 0xA; break;
//             case 'x': case 'X': chip8_key = 0x0; break;
//             case 'c': case 'C': chip8_key = 0xB; break;
//             case 'v': case 'V': chip8_key = 0xF; break;
//         }
        
//         if (chip8_key <= 0xF) {
//             handle_key_press(chip8_key);
//         }
//     }
// }