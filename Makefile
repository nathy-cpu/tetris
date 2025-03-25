# Makefile for Raylib project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -std=c11 -Wall -Werror -Wextra -Wswitch-enum -Wunreachable-code -fsanitize=undefined -fsanitize=address -I/usr/local/include

# Linker flags
LDFLAGS = -L/usr/local/lib -lraylib -lm -lpthread -ldl -lrt -lX11

# Source files
SRCS = $(wildcard src/*.c)

# Executable name
TARGET = tetris

# Default target
all: $(TARGET)

# Building the executable directly from source files
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Format target using clang-format with WebKit style
format:
	find src -iname '*.c' -o -iname '*.h' | xargs clang-format -i -style=Webkit

# Run target to build and execute the program
run: $(TARGET)
	./$(TARGET)

# Clean target
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean run format
