#include "colors.h"

#include <raylib.h>
#include <stdlib.h>

#include "array.h"

Array GetCellColors(Arena* arena)
{
    Array colors = Array_Init(arena, 8, sizeof(Color));
    Color c[8] = { darkGrey, green, red, orange, yellow, purple, cyan, blue };
    for (size_t i = 0; i < 8; i++) {
        Array_Set(&colors, i, &c[i]);
    }
    return colors;
}
