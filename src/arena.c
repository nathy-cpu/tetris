#include "tetris.h"
#include <stdlib.h>

// Define the alignment boundary
#define ALIGNMENT (sizeof(max_align_t))

// Round up `size` to the nearest multiple of `alignment`
static inline size_t align_up(size_t size, size_t alignment)
{
    return (size + alignment - 1) & ~(alignment - 1);
}

Arena* Arena_Init(size_t size)
{
    Arena* arena = (Arena*)malloc(sizeof(Arena));
    if (!arena)
        return NULL;

    arena->memory = (uint8_t*)malloc(size);
    if (!arena->memory) {
        free(arena);
        return NULL;
    }

    arena->size = size;
    arena->head = 0;
    arena->tail = size;
    return arena;
}

void* Arena_Allocate(Arena* arena, size_t size)
{
    if (arena->head + size > arena->tail - sizeof(size_t)) {
        return NULL;
    }

    void* ptr = &arena->memory[arena->head];
    arena->head += size;

    arena->tail -= sizeof(size_t);
    *((size_t*)&arena->memory[arena->tail]) = size;

    return ptr;
}

void* Arena_AllocateAligned(Arena* arena, size_t size)
{
    // Align the size of the object
    size_t aligned_size = align_up(size, ALIGNMENT);
    size_t aligned_head = align_up(arena->head, ALIGNMENT);

    if (aligned_head + aligned_size > arena->tail - sizeof(size_t)) {
        return NULL;
    }

    void* ptr = &arena->memory[aligned_head];
    arena->head = aligned_head + aligned_size;

    arena->tail -= sizeof(size_t);
    *((size_t*)&arena->memory[arena->tail]) = aligned_size;

    return ptr;
}

void Arena_Pop(Arena* arena)
{
    if (arena->tail >= arena->size) {
        return;
    }

    size_t size = *((size_t*)&arena->memory[arena->tail]);
    arena->tail += sizeof(size_t);
    arena->head -= size;
}

void Arena_Reset(Arena* arena)
{
    arena->head = 0;
    arena->tail = arena->size;
}

void Arena_Free(Arena* arena)
{
    free(arena->memory);
    free(arena);
}