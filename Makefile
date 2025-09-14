CC = g++
CXXFLAGS = -Wall -Wextra -Werror -g -Iinclude

TARGET = chip8
SRCS = src/chip8.cpp src/main.cpp src/cpu.cpp src/display.cpp src/input.cpp src/memory.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean fclean re run
