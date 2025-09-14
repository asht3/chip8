CC = g++
CXXFLAGS = -Wall -Wextra -Werror -g -Iinclude

TARGET = chip8
SRCS = src/chip8.cpp src/main.cpp src/cpu.cpp src/display.cpp src/input.cpp src/memory.cpp
OBJS = $(patsubst src/%.cpp, obj/%.o, $(SRCS))

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS)

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) obj/*.o

fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean fclean re run
