#include "../include/display.hpp"
#include <algorithm>

Display::Display() {
    clear();  // Initialize with blank display
}

void Display::clear() {
    std::fill(std::begin(pixels), std::end(pixels), false);
    draw_flag = true;
}

// uint8_t* Display::get_pixels() {
//     return pixels;
// }

bool Display::get_pixel(uint8_t x, uint8_t y) {
    return pixels[x + y * 64];
}

void Display::set_pixel(uint8_t x, uint8_t y, bool value) {
    pixels[x + y * 64] = value;
}

bool Display::xor_pixel(uint8_t x, uint8_t y) {
    int index = x + y * 64;
    bool old_value = pixels[index];
    pixels[index] = pixels[index] ^ true;  // XOR with 1
    return old_value && !pixels[index];    // Collision if was on and now off
}

void Display::flip_pixel(uint8_t x, uint8_t y) {
    pixels[x + y * 64] = !pixels[x + y * 64];
}