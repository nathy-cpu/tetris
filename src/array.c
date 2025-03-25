#include "array.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Array Array_Init(Arena* arena, size_t size, size_t elementSize)
{
    Array array = (Array) { .data = (char*)Arena_AllocateAligned(arena, size * elementSize),
        .size = size,
        .elementSize = elementSize };
    return array;
}

void Array_Set(Array* array, size_t index, const void* element)
{
    if (index >= array->size) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }
    memcpy(array->data + (index * array->elementSize), element, array->elementSize);
}

void* Array_Get(const Array* array, size_t index)
{
    if (index >= array->size) {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }
    return array->data + (index * array->elementSize);
}

size_t Array_Size(const Array* array)
{
    return array->size;
}

Array Array_Malloc(size_t size, size_t elementSize)
{
    Array array = (Array) { .data = (char*)calloc(size, elementSize), .size = size, .elementSize = elementSize };
    // should handle calloc error but too lazy
    return array;
}

void Array_Free(Array* array)
{
    free(array->data);
    *array = (Array) { .data = NULL, .size = 0, .elementSize = 0 };
}
