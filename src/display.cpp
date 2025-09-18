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

bool Display::draw_sprite(uint8_t x, uint8_t y, uint8_t* sprite, uint8_t height) {
    bool collision = false;
    for (int row = 0; row < height; row++) {
        uint8_t sprite_row = sprite[row];
        for (int col = 0; col < 8; col++) {
            if ((sprite_row & (0x80 >> col)) != 0) {  // Check if the bit is set
                int px = (x + col) % 64;              // Wrap around horizontally
                int py = (y + row) % 32;              // Wrap around vertically
                if (xor_pixel(px, py)) {
                    collision = true;                  // Collision detected
                }
            }
        }
    }
    draw_flag = true;
    return collision;
}

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

bool Display::needs_redraw() {
    return draw_flag;
}

void Display::clear_redraw_flag() {
    draw_flag = false;
}