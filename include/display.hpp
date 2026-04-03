#pragma once
#include <cstdint>
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

class Display {
    private:
        static const int WIDTH = 64;
        static const int HEIGHT = 32;
        uint8_t pixels[WIDTH * HEIGHT];
        bool draw_flag;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

            bool vblank_enabled = true;
    std::chrono::steady_clock::time_point last_vblank_time;

    public:
        Display();
        void clear();
        void set_draw_flag();
        bool draw_sprite(uint8_t x, uint8_t y, uint8_t* sprite, uint8_t height);
        bool get_pixel(uint8_t x, uint8_t y) const;
        void set_pixel(uint8_t x, uint8_t y, bool value);
        bool xor_pixel(uint8_t x, uint8_t y);
        void flip_pixel(uint8_t x, uint8_t y);
        bool needs_redraw();
        void clear_redraw_flag();
        void wait_for_vblank();
        void enable_vblank(bool enable);

        // For SDL rendering
        void init_sdl(const char* title, int scale);
        void cleanup_sdl();
        void render_to_sdl(int scale);
};