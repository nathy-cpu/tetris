#include "tetris.h"

Position Position_Init(uint8_t row, uint8_t column)
{
    Position position = (Position) { .row = row, .column = column };
    return position;
}