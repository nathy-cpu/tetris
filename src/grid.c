#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Grid* Grid_Init(void)
{
    Grid* grid = malloc(sizeof(Grid));
    grid->numRows = GRID_ROWS;
    grid->numCols = GRID_COLUMNS;
    memset(grid->grid, 0, sizeof(grid->grid));
    return grid;
}

void Grid_Reset(Grid* grid)
{
    memset(grid->grid, 0, sizeof(grid->grid));
}

void Grid_Free(Grid* grid)
{
    if (grid) {
        free(grid);
    }
}

void Grid_Print(const Grid* grid)
{
    for (int row = 0; row < grid->numRows; row++) {
        for (int column = 0; column < grid->numCols; column++) {
            printf("%i ", grid->grid[row][column]);
        }
        printf("\n");
    }
}

void Grid_Draw(const Grid* grid, Texture2D tileSpriteSheet)
{
    DrawRectangle(
        GRID_PADDING,
        GRID_PADDING,
        (GRID_CELL_SIZE * GRID_COLUMNS),
        SCREEN_HEIGHT - GRID_PADDING * 2,
        darkGrey);

    for (int row = 0; row < grid->numRows; row++) {
        for (int column = 0; column < grid->numCols; column++) {
            int cellValue = grid->grid[row][column];
            if (cellValue != 0) {
                DrawTexturePro(tileSpriteSheet,
                    (Rectangle) { (cellValue - 1) * 16, 0, 16, 16 },
                    (Rectangle) { column * GRID_CELL_SIZE + GRID_PADDING, row * GRID_CELL_SIZE + GRID_PADDING, GRID_CELL_SIZE - 1,
                        GRID_CELL_SIZE - 1 },
                    (Vector2) { 0, 0 }, 0, WHITE);
            }
        }
    }
}

bool Grid_IsCellOutside(const Grid* grid, int8_t row, int8_t column)
{
    if (row >= 0 && row < grid->numRows && column >= 0 && column < grid->numCols) {
        return false;
    }
    return true;
}

bool Grid_IsEmpty(const Grid* grid, uint8_t row, uint8_t column)
{
    if (grid->grid[row][column] == 0) {
        return true;
    }
    return false;
}

bool Grid_IsRowFull(const Grid* grid, uint8_t row)
{
    for (int column = 0; column < grid->numCols; column++) {
        if (grid->grid[row][column] == 0) {
            return false;
        }
    }
    return true;
}

void Grid_ClearRow(Grid* grid, uint8_t row)
{
    for (int column = 0; column < grid->numCols; column++) {
        grid->grid[row][column] = 0;
    }
}

void Grid_MoveRowDown(Grid* grid, uint8_t row, uint8_t numRows)
{
    for (int column = 0; column < grid->numCols; column++) {
        grid->grid[row + numRows][column] = grid->grid[row][column];
        grid->grid[row][column] = 0;
    }
}

uint8_t Grid_ClearFullRows(Grid* grid)
{
    uint8_t completed = 0;
    for (int8_t row = grid->numRows - 1; row >= 0; row--) {
        if (Grid_IsRowFull(grid, row)) {
            Grid_ClearRow(grid, row);
            completed++;
        } else if (completed > 0) {
            Grid_MoveRowDown(grid, row, completed);
        }
    }
    return completed;
}