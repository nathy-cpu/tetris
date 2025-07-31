# Makefile for Raylib project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -std=c11 -Wall -Werror -Wextra -Wswitch-enum -Wunreachable-code -fsanitize=undefined -fsanitize=address

# Linker flags
LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

# Source files
SRCS = $(wildcard src/*.c)

# Executable name
BIN_DIR = bin
TARGET = $(BIN_DIR)/tetris

# Ensure bin/ exists
$(shell mkdir -p $(BIN_DIR))

# Default target
all: $(TARGET)

build: clean $(TARGET)

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

lint: format
	clang-tidy $(SRCS) $(TEST_SRCS) -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus* -- $(CFLAGS) $(LDFLAGS)

# Phony targets
.PHONY: all clean build run format lint