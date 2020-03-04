#pragma once

#include "../Engine/vec2i.h"
#include "Tetromino.h"

extern const SDL_Rect background_rect;
extern const SDL_Rect score_rect;
extern const SDL_Rect level_rect;
extern const SDL_Rect lines_rect;
extern const SDL_Rect block_view_rect;
extern const SDL_Rect play_area_rect;

extern const SDL_Rect digits[10];


extern SDL_Texture *tetris;

uint32_t number_of_digits(int value);

void RenderIntInRect(SDL_Renderer *renderer, int value, const SDL_Rect rect);

void DrawTetromino(SDL_Renderer *renderer, TetrominoType type);

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22
#define BOARD_OFFSET 4

typedef struct Cell Cell;
struct Cell {
	SDL_Rect source;
	int rotation;
	bool is_falling;
	bool is_empty;
};

void swap_cells(Cell *a, Cell *b);
Cell CreateCell();
void CellDraw(const Cell *cell, SDL_Renderer *renderer, const SDL_Point pos);

extern const uint32_t STARTING_MOVE_DOWN_PERIOD;
typedef struct Board Board;
struct Board {
	Cell cells[BOARD_WIDTH][BOARD_HEIGHT];
	uint32_t ticks;
	uint32_t move_down_period;
	TetrominoType next_tromino;
	int overriden_blocks;
};

Board CreateBoard();
bool BoardCantMoveAllFallinBlocks(const Board *board, const SDL_Point offset);

void BoardMoveAllFallingBlocks(Board *board, const SDL_Point offset);
bool Board_can_move_all_falling_blocks_down(const Board *board);
void Board_move_all_falling_blocks_down(Board *board);
// returns the number of cleared rows
uint32_t Board_clear_filled_rows(Board *board);
void Board_set_all_falling_blocks_not_falling(Board *board);
void BoardAddTromino(Board *board);
uint32_t Board_update(Board *board, const uint32_t delta_ticks);
bool Board_can_rotate_falling_blocks(const Board *board);
void Board_rotate_falling_blocks(Board *board);
void Board_draw(const Board *board, SDL_Renderer *renderer);