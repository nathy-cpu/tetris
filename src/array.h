#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

#include "arena.h"

typedef struct
{
    size_t size;
    size_t elementSize;
    char* data;
} Array;

#define ARRAY_EMPTY (Array) { .data = NULL, .size = 0, .elementSize = 0 }

Array Array_Init(Arena* arena, size_t size, size_t elementSize);

void Array_Set(Array* array, size_t index, const void* element);

void* Array_Get(const Array* array, size_t index);

size_t Array_Size(const Array* array);

Array Array_Malloc(size_t size, size_t elementSize);

void Array_Free(Array* array);

#endif
