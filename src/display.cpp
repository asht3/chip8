#include "../include/display.hpp"
#include <algorithm>

Display::Display() {
    clear();  // Initialize with blank display
}

void Display::clear() {
    std::fill(std::begin(pixels), std::end(pixels), false);
    draw_flag = true;
}

uint8_t* Display::get_pixels() {
    return pixels;
}