#ifndef GRID_H
#define GRID_H

#include <raylib.h>
#include <stdint.h>

#include "array.h"

typedef struct
{
    Array colors;
    int cellSize;
    uint8_t numRows;
    uint8_t numCols;
    uint8_t grid[20][10];

} Grid;

Grid* Grid_Init(Arena* arena);

void Grid_Reset(Grid* grid);

Color Grid_GetCellColor(const Grid* grid, uint8_t cellValue);

void Grid_Print(const Grid* grid);

void Grid_Draw(const Grid* grid);

bool Grid_IsCellOutside(const Grid* grid, int8_t row, int8_t column);

bool Grid_IsEmpty(const Grid* grid, uint8_t row, uint8_t column);

bool Grid_IsRowFull(const Grid* grid, uint8_t row);

void Grid_ClearRow(Grid* grid, uint8_t row);

void Grid_MoveRowDown(Grid* grid, uint8_t row, uint8_t numRows);

uint8_t Grid_ClearFullRows(Grid* grid);

#endif
