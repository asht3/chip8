#include "../include/input.hpp"
#include <algorithm>

Input::Input() {
    reset();  // Initialize with no keys pressed
}

void Input::reset() {
    std::fill(std::begin(keys), std::end(keys), false);
}

// bool* Input::get_keys() {
//     return keys;
// }

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