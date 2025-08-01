#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Board* Board_Init(void)
{
    Board* board = malloc(sizeof(Board));
    board->numRows = BOARD_ROWS;
    board->numCols = BOARD_COLUMNS;
    memset(board->grid, 0, sizeof(board->grid));
    return board;
}

void Board_Reset(Board* board)
{
    memset(board->grid, 0, sizeof(board->grid));
}

void Board_Free(Board* board)
{
    if (board) {
        free(board);
    }
}

void Board_Print(const Board* board)
{
    for (int row = 0; row < board->numRows; row++) {
        for (int column = 0; column < board->numCols; column++) {
            printf("%i ", board->grid[row][column]);
        }
        printf("\n");
    }
}

void Board_Draw(const Board* board, Texture2D tileSpriteSheet)
{
    DrawRectangle(
        BOARD_PADDING,
        BOARD_PADDING,
        (BOARD_CELL_SIZE * BOARD_COLUMNS),
        SCREEN_HEIGHT - BOARD_PADDING * 2,
        darkGrey);

    for (int row = 0; row < board->numRows; row++) {
        for (int column = 0; column < board->numCols; column++) {
            int cellValue = board->grid[row][column];
            if (cellValue != 0) {
                DrawTexturePro(tileSpriteSheet,
                    (Rectangle) { (cellValue - 1) * SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE },
                    (Rectangle) { column * BOARD_CELL_SIZE + BOARD_PADDING, row * BOARD_CELL_SIZE + BOARD_PADDING, BOARD_CELL_SIZE,
                        BOARD_CELL_SIZE },
                    (Vector2) { 0, 0 }, 0, WHITE);
            }
        }
    }
}

bool Board_IsCellOutside(const Board* board, int8_t row, int8_t column)
{
    if (row >= 0 && row < board->numRows && column >= 0 && column < board->numCols) {
        return false;
    }
    return true;
}

bool Board_IsEmpty(const Board* board, uint8_t row, uint8_t column)
{
    if (board->grid[row][column] == 0) {
        return true;
    }
    return false;
}

bool Board_IsRowFull(const Board* board, uint8_t row)
{
    for (int column = 0; column < board->numCols; column++) {
        if (board->grid[row][column] == 0) {
            return false;
        }
    }
    return true;
}

void Board_ClearRow(Board* board, uint8_t row)
{
    for (int column = 0; column < board->numCols; column++) {
        board->grid[row][column] = 0;
    }
}

void Board_MoveRowDown(Board* board, uint8_t row, uint8_t numRows)
{
    for (int column = 0; column < board->numCols; column++) {
        board->grid[row + numRows][column] = board->grid[row][column];
        board->grid[row][column] = 0;
    }
}

uint8_t Board_ClearFullRows(Board* board)
{
    uint8_t completed = 0;
    for (int8_t row = board->numRows - 1; row >= 0; row--) {
        if (Board_IsRowFull(board, row)) {
            Board_ClearRow(board, row);
            completed++;
        } else if (completed > 0) {
            Board_MoveRowDown(board, row, completed);
        }
    }
    return completed;
}
