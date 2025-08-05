# Makefile for Raylib project

# Import .env file if present
ifneq (,$(wildcard .env))
	include .env
	export
endif

# Default build type (debug)
BUILD ?= debug

# Platform detection
OS ?= $(OS)

# Whether to enable address/ub sanitizers
ENABLE_SAN ?= false

# Compiler
CC ?= clang

# Compiler flags
CFLAGS = -std=c99 -Wall -Werror -Wextra -Wswitch-enum -Wunreachable-code

# Platform-specific settings
ifeq ($(OS),Linux)
	CFLAGS += -D_POSIX_C_SOURCE=200809L
	LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(OS),Darwin)
	CFLAGS += -D_DARWIN_C_SOURCE
	LDFLAGS = -lraylib -lpthread -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework CoreAudio -framework AudioToolbox -framework ForceFeedback -framework SystemConfiguration -framework CoreGraphics
endif
ifeq ($(OS),Windows_NT)
	CFLAGS += -D_WIN32_WINNT=0x0600 -m64
	LDFLAGS = -lraylib -lwinmm -lgdi32 -m64
endif

# Build-specific flags
ifeq ($(BUILD),release)
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS += -s
else
	CFLAGS += -g -O0 
endif

# Enable ASAN and UBSAN sanitizers if configured
ifeq ($(ENABLE_SAN),true)
	CFLAGS += -fsanitize=undefined -fsanitize=address
endif

# Source files
SRCS = $(wildcard src/*.c)

# Executable name
BIN_DIR = bin
ifeq ($(OS),Windows_NT)
    TARGET = $(BIN_DIR)/tetris.exe
else
    TARGET = $(BIN_DIR)/tetris
endif

# Default target
all: build

build: clean $(BIN_DIR) $(TARGET)

# Building the executable directly from source files
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Ensuring bin directory is present
$(BIN_DIR):
ifeq ($(OS),Windows_NT)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
else
	@mkdir -p $(BIN_DIR)
endif

# Format target using clang-format with WebKit style
ifeq ($(OS),Windows_NT)
format:
	@for %%f in (src\*.c src\*.h) do clang-format -i -style=Webkit "%%f"
else
format:
	find src -iname '*.c' -o -iname '*.h' | xargs clang-format -i -style=Webkit
endif

# Run target to build and execute the program
run: build
	./$(TARGET)

# Clean target
ifeq ($(OS),Windows_NT)
clean:
	@if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)
else
clean:
	@rm -rf $(BIN_DIR)
endif

lint: format
	clang-tidy $(SRCS) -checks=*,-clang-analyzer-cplusplus*,-readability-*,-modernize-*,-google-*,-llvm-*,-misc-* -- $(CFLAGS) $(LDFLAGS)

# Phony targets
.PHONY: all clean build run format lint