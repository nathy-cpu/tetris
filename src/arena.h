#ifndef ARENA_H
#define ARENA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define GB1 (1024 * 1024 * 1024)
#define MB512 (1024 * 1024 * 1024) / 2
#define MB256 (1024 * 1024 * 1024) / 4
#define MB128 (1024 * 1024 * 1024) / 8
#define MB64 (1024 * 1024 * 1024) / 16
#define MB32 (1024 * 1024 * 1024) / 32

typedef struct {
    size_t size;
    size_t head;
    size_t tail;
    uint8_t* memory;
} Arena;

Arena* Arena_Init(size_t size);

void* Arena_Allocate(Arena* arena, size_t size);

void* Arena_AllocateAligned(Arena* arena, size_t size);

void Arena_Pop(Arena* arena);

void Arena_Reset(Arena* arena);

void Arena_Free(Arena* arena);

#endif
