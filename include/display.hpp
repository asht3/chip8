#include <cstdint>

class Display {
    private:
        static const int WIDTH = 64;
        static const int HEIGHT = 32;
        bool pixels[WIDTH][HEIGHT];
        bool draw_flag;

    public:
        Display();
        void clear();
        bool draw_sprite(uint8_t x, uint8_t y, uint8_t* sprite, uint8_t height);
        bool get_pixel(uint8_t x, uint8_t y);
        void set_pixel(uint8_t x, uint8_t y, bool value);
        bool needs_redraw();
        void clear_redraw_flag();
};