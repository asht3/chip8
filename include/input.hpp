#pragma once
#include <cstdint>
#include <SDL2/SDL.h>

class Input {
    private:
        bool keys[16];
        bool waiting_for_key;
        uint8_t wait_register;

    public:
        Input();
        void reset();
        void handle_input_sdl(const SDL_Event& event, Input& chip8_input);
        void set_key(uint8_t key, bool pressed);
        bool is_pressed(int key);
        uint8_t wait_for_keypress();

 
        // bool is_waiting_for_key() const;         // Check if in wait state
        // void set_wait_state(uint8_t register_index);  // Set wait state
        // void handle_key_press(uint8_t key);      // Handle key during wait state
        // uint8_t get_wait_register() const;      // Get the register index to store key
};