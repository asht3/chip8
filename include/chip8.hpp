#include "display.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include "input.hpp"

class Chip8 {
    private:
        CPU cpu;
        Memory memory;
        Display display;
        Input input;
        bool running;

    public:
        Chip8();
        void load_rom(const char* filename);
        void emulate_cycle();
        void run();
        void stop();

        Display& get_display();
        bool is_running();
};