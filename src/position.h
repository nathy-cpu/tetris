#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

typedef struct
{
    uint8_t row;
    uint8_t column;

} Position;

Position Position_Init(uint8_t row, uint8_t column);

#endif
