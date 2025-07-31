# Makefile for Raylib project

# Default build type (debug)
BUILD ?= debug

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=gnu11 -Wall -Werror -Wextra -Wswitch-enum -Wunreachable-code

# Platform detection
UNAME_S := $(shell uname -s)

# Platform-specific settings
ifeq ($(UNAME_S),Linux)
    CFLAGS += -D_POSIX_C_SOURCE=200809L
	LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -D_DARWIN_C_SOURCE
    LDFLAGS = -lraylib -lpthread -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework CoreAudio -framework AudioToolbox -framework ForceFeedback -framework SystemConfiguration -framework CoreGraphics
endif
ifeq ($(UNAME_S),Windows_NT)
    CFLAGS += -D_WIN32_WINNT=0x0600
    LDFLAGS = -lraylib -lwinmm -lgdi32
endif

# Build-specific flags
ifeq ($(BUILD),release)
    CFLAGS += -O3 -DNDEBUG
    LDFLAGS += -s
else
    CFLAGS += -g -O0 -fsanitize=undefined -fsanitize=address
endif

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