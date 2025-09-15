#pragma once
#include <cstdint>

class Input {
    private:
        bool keys[16];
    public:
        Input();
        bool* get_keys();
        void set_key(uint8_t key, bool pressed);
        bool is_key_pressed(int key);
        uint8_t wait_for_keypress();
};