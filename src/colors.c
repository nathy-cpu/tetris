#include "tetris.h"

#define COLORS_NUM 8

Array GetCellColors(void) {
    Array colors = Array_Init(COLORS_NUM, sizeof(Color));
    Array_Set(&colors, 0, &darkGrey);
    Array_Set(&colors, 1, &blue);
    Array_Set(&colors, 2, &orange);
    Array_Set(&colors, 3, &cyan);
    Array_Set(&colors, 4, &yellow);
    Array_Set(&colors, 5, &green);
    Array_Set(&colors, 6, &purple);
    Array_Set(&colors, 7, &red);
    return colors;
}