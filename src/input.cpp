#include "../include/input.hpp"
#include <algorithm>

Input::Input() {
    reset();  // Initialize with no keys pressed
}

void Input::reset() {
    std::fill(std::begin(keys), std::end(keys), false);
}

bool* Input::get_keys() {
    return keys;
}

bool Input::is_pressed(int key) {
    if (key < 0 || key >= 16) {
        return false;
    }
    return keys[key];
}