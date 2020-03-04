#include <assert.h>
#include <complex.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../Engine/App.h"
#include "Tetris.h"

const SDL_Rect background_rect = {   0,   0, 160, 144 };
const SDL_Rect score_rect      = { 113,  24,  40,   8 };
const SDL_Rect level_rect      = { 113,  56,  36,   8 };
const SDL_Rect lines_rect      = { 113,  80,  36,   8 };
const SDL_Rect block_view_rect = { 120, 104,  32,  32 };
const SDL_Rect play_area_rect  = {  16,   0,  80, 144 };

const SDL_Rect digits[10] = {
	{ 168 + BLOCK_LENGTH * 0, 76, BLOCK_LENGTH, BLOCK_LENGTH },
	{ 168 + BLOCK_LENGTH * 1, 76, BLOCK_LENGTH, BLOCK_LENGTH },
	{ 168 + BLOCK_LENGTH * 2, 76, BLOCK_LENGTH, BLOCK_LENGTH },
	{ 168 + BLOCK_LENGTH * 3, 76, BLOCK_LENGTH, BLOCK_LENGTH },
	{ 168 + BLOCK_LENGTH * 4, 76, BLOCK_LENGTH, BLOCK_LENGTH },
	{ 168 + BLOCK_LENGTH * 5, 76, BLOCK_LENGTH, BLOCK_LENGTH },
	{ 168 + BLOCK_LENGTH * 6, 76, BLOCK_LENGTH, BLOCK_LENGTH },
	{ 168 + BLOCK_LENGTH * 7, 76, BLOCK_LENGTH, BLOCK_LENGTH },
	{ 168 + BLOCK_LENGTH * 8, 76, BLOCK_LENGTH, BLOCK_LENGTH },
	{ 168 + BLOCK_LENGTH * 9, 76, BLOCK_LENGTH, BLOCK_LENGTH },
};


SDL_Texture *tetris = NULL;

uint32_t number_of_digits(int value) {
	assert(value >= 0);

	if (value == 0) {
		return 1;
	} else {
		return floor(log10(abs(value) + 1));
	}
}

void RenderIntInRect(SDL_Renderer *renderer, int value, const SDL_Rect rect) {
	assert(value >= 0);
	assert(renderer);
	// asserts that the length of the digits is less than 4
	assert((int)(number_of_digits(value)) <= 4);

	int x_pos = rect.x + rect.w - digits[0].w;

	if (value == 0) {
		SDL_Rect dest = {
			x_pos,
			rect.y,
			digits[0].w,
			digits[0].h
		};
		SDL_RenderCopy(renderer, tetris, &digits[0], &dest);
		return;
	}

	for (; x_pos >= rect.x && value > 0; x_pos -= digits[0].w, value /= 10) {
		SDL_Rect dest = {
			x_pos,
			rect.y,
			digits[0].w,
			digits[0].h
		};
		SDL_RenderCopy(renderer, tetris, &digits[value % 10], &dest);
	}
}

void DrawTetromino(SDL_Renderer *renderer, TetrominoType type) {
	assert(renderer);
	assert((uint32_t)(type) < 7);

	const SDL_Rect  *tetromino_rect = &tetromino_source_rects[(uint32_t)(type)][0];
	SDL_Point top_left = GetTopLeft(type);
	SDL_Point bottom_right = GetBottomRight(type);
	SDL_Rect bounds = (SDL_Rect){
		top_left.x,
		top_left.y,
		bottom_right.x - top_left.x,
		bottom_right.y - top_left.y
	};
	int x = (int)((block_view_rect.w - bounds.w) / 2.f);
	int y = (int)((block_view_rect.h - bounds.h) / 2.f);
	for (int i = 0; i < 4; ++i) {
		SDL_Rect source = tetromino_rect[i];
		SDL_Rect dest = {
			(int)(source.x - top_left.x + block_view_rect.x + x),
			(int)(source.y - top_left.y + block_view_rect.y + y),
			(int)(BLOCK_LENGTH),
			(int)(BLOCK_LENGTH),
		};

		SDL_RenderCopy(renderer, tetris, &source, &dest);
	}
}

void swap_cells(Cell *a, Cell *b) {
	assert(a);
	assert(b);
	Cell c = *a;
	*a = *b;
	*b = c;
}
Cell CreateCell() {
	return (Cell) {
		{ 0, 0, 0, 0 },
		0,
		false,
		true
	};
}
void CellDraw(const Cell *cell, SDL_Renderer *renderer, const SDL_Point pos) {
	assert(cell);

	if (cell->is_empty) {
		return;
	}

	SDL_Rect dest = {
		(int)(pos.x * BLOCK_LENGTH) + play_area_rect.x,
		(int)(pos.y * BLOCK_LENGTH) + play_area_rect.y,
		BLOCK_LENGTH,
		BLOCK_LENGTH
	};
	SDL_RenderCopyEx(renderer, tetris, &cell->source, &dest, -90.0 * cell->rotation, NULL, SDL_FLIP_NONE);
}

const uint32_t STARTING_MOVE_DOWN_PERIOD = 1000;

Board CreateBoard() {
	Board board;

	board.ticks = 0;
	board.move_down_period = STARTING_MOVE_DOWN_PERIOD;

	for (int y = 0; y < BOARD_HEIGHT; ++y)
		for (int x = 0; x < BOARD_WIDTH; ++x) {
			board.cells[x][y] = CreateCell();
		}
	board.next_tromino = (TetrominoType)(rand() % 7);
	board.overriden_blocks = 0;
	return board;
}
bool BoardCantMoveAllFallinBlocks(const Board *board, const SDL_Point offset) {
	assert(board);
	assert(abs(offset.x) + abs(offset.y) == 1);

	for (int y = 0; y < BOARD_HEIGHT; ++y)
		for (int x = 0; x < BOARD_WIDTH; ++x) {
			const int x_dst = x + offset.x;
			const int y_dst = y + offset.y;
			if (board->cells[x][y].is_falling) {
				if (
					x_dst < 0 || 
					x_dst >= BOARD_WIDTH || 
					y_dst < 0 || 
					y_dst >= BOARD_HEIGHT
				) {
					return false;
				}

				if (!board->cells[x_dst][y_dst].is_empty && !board->cells[x_dst][y_dst].is_falling) {
					return false;
				}
			}
		}
	return true;
}

int ri_backward_init(uint32_t length) { return (int)(length) - 1; };
int ri_forward_init(uint32_t length) { return 0; };
bool ri_backward_cond(int z, uint32_t length) { return z >= 0; };
bool ri_forward_cond(int z, uint32_t length) { return z < (int)(length); };
int ri_backward_iter(int z) { return z - 1; };
int ri_forward_iter(int z) { return z + 1; };
void BoardMoveAllFallingBlocks(Board *board, const SDL_Point offset) {
	assert(BoardCantMoveAllFallinBlocks(board, offset));


	int (*const x_init)(uint32_t)       = !(offset.x >= 0) ? ri_forward_init : ri_backward_init;
	bool (*const x_cond)(int, uint32_t) = !(offset.x >= 0) ? ri_forward_cond : ri_backward_cond;
	int (*const x_iter)(int)            = !(offset.x >= 0) ? ri_forward_iter : ri_backward_iter;

	int (*const y_init)(uint32_t)       = !(offset.y >= 0) ? ri_forward_init : ri_backward_init;
	bool (*const y_cond)(int, uint32_t) = !(offset.y >= 0) ? ri_forward_cond : ri_backward_cond;
	int (*const y_iter)(int)            = !(offset.y >= 0) ? ri_forward_iter : ri_backward_iter;

	for (int y = y_init(BOARD_HEIGHT); y_cond(y, BOARD_HEIGHT); y = y_iter(y)) {
		for (int x = x_init(BOARD_WIDTH); x_cond(x, BOARD_WIDTH); x = x_iter(x)) {
			if (board->cells[x][y].is_falling) {
				assert(0 <= x && x <= BOARD_WIDTH);
				assert(0 <= y && y <= BOARD_HEIGHT);
				swap_cells(&board->cells[x][y], &board->cells[x + offset.x][y + offset.y]);
			}
		}
	}
}
bool Board_can_move_all_falling_blocks_down(const Board *board) {
	return BoardCantMoveAllFallinBlocks(board, (SDL_Point){ 0, +1 });
}
void Board_move_all_falling_blocks_down(Board *board) {
	return BoardMoveAllFallingBlocks(board, (SDL_Point){ 0, +1 });
}
void Board_clear_row(Board *board, const uint32_t row) {
	assert(board);
	assert(row < BOARD_HEIGHT);

	for (int x = 0; x < BOARD_WIDTH; ++x) {
		board->cells[x][row] = CreateCell();
	}

	for (int y = row; y > 0; --y) {
		for (int x = 0; x < BOARD_WIDTH; ++x) {
			swap_cells(&board->cells[x][y], &board->cells[x][y - 1]);
		}
	}
}
uint32_t Board_clear_filled_rows(Board *board) {
	assert(board);

	uint32_t cleared_rows = 0;
	for (int y = 0; y < BOARD_HEIGHT; ++y) {
		bool row_is_filled = true;
		for (int x = 0; x < BOARD_WIDTH; ++x) {
			row_is_filled &= !board->cells[x][y].is_empty;
		}
		if (row_is_filled) {
			Board_clear_row(board, y);
			++cleared_rows;
		}
	}

	return cleared_rows;
}
void Board_set_all_falling_blocks_not_falling(Board *board) {
	for (int y = 0; y < BOARD_HEIGHT; ++y)
		for (int x = 0; x < BOARD_WIDTH; ++x) {
			if (board->cells[x][y].is_falling) {
				board->cells[x][y].is_falling = false;
			}
		}
}

void BoardAddTromino(Board *board) {
	SDL_Point top_left = GetTopLeft(board->next_tromino);
	SDL_Point bottom_right = GetBottomRight(board->next_tromino);
	const int width = (int)((bottom_right.x - top_left.x) / BLOCK_LENGTH);

	assert(width > 0);

	const uint32_t x_min = ((BOARD_WIDTH - width) / 2);
	const uint32_t y_min = 0;
	const SDL_Rect *source_rects = &tetromino_source_rects[(uint32_t)(board->next_tromino)][0];
	bool overode_a_block = false;
	for (int i = 0; i < TETRA; ++i) {
		SDL_Rect rect = source_rects[i];
		uint32_t x = (uint32_t)((float)(rect.x - top_left.x) / BLOCK_LENGTH) + x_min;
		uint32_t y = (uint32_t)((float)(rect.y - top_left.y) / BLOCK_LENGTH) + y_min;

		if (!board->cells[x][y + BOARD_OFFSET].is_empty) {
			overode_a_block = true;
		}

		board->cells[x][y + BOARD_OFFSET].source = rect;
		board->cells[x][y + BOARD_OFFSET].is_falling = true;
		board->cells[x][y + BOARD_OFFSET].is_empty = false;
	}

	if (overode_a_block) {
		++board->overriden_blocks;
	}
	
	board->next_tromino = (TetrominoType)(rand() % 7);
}
uint32_t CalcPoints(uint32_t rows) {
	assert(0 <= rows && rows <= 4);

	return rows * 100;
}
uint32_t Board_update(Board *board, const uint32_t delta_ticks) {
	assert(board);

	uint32_t cleared_rows = 0;
	board->ticks += delta_ticks;
	if (board->ticks >= board->move_down_period) {
		board->ticks -= board->move_down_period;
		if (Board_can_move_all_falling_blocks_down(board)) {
			Board_move_all_falling_blocks_down(board);
		} else {
			Board_set_all_falling_blocks_not_falling(board);
			cleared_rows = Board_clear_filled_rows(board);
			BoardAddTromino(board);
		}
	}

	if (board->overriden_blocks > 3) {
		SDL_Event event;
		SDL_zero(event);
		event.type = GAMEOVER_EVENT;
		SDL_PushEvent(&event);
	}

	return CalcPoints(cleared_rows);
}
// Make sure to free return if not NULL return is a dimensions.x * dimensions.y array
Cell* get_new_top_left(const Board *board, vec2i *tl, vec2i *dimensions) {
	assert(board);

	vec2i top_left = { INT32_MAX, INT32_MAX };
	vec2i bottom_right = { INT32_MIN, INT32_MIN };
	for (int32_t j = 0; j < BOARD_HEIGHT; ++j)
		for (int32_t i = 0; i < BOARD_WIDTH; ++i) {
			if (board->cells[i][j].is_falling) {
				top_left.x = int_min(top_left.x, i);
				top_left.y = int_min(top_left.y, j);
				bottom_right.x = int_max(bottom_right.x, i);
				bottom_right.y = int_max(bottom_right.y, j);
			}
		}


	assert(top_left.x <= bottom_right.x);
	assert(top_left.y <= bottom_right.y);

	const int width = bottom_right.x - top_left.x + 1;
	const int height = bottom_right.y - top_left.y + 1;

	Cell *pieces = malloc(sizeof(Cell) * width * height);
#define PIECES(i, j) pieces[(i) + (j) * width]

	for (int32_t j = top_left.y; j <= bottom_right.y; ++j)
		for (int32_t i = top_left.x; i <= bottom_right.x; ++i) {
			PIECES(i - top_left.x, j - top_left.y) = CreateCell();
			if (board->cells[i][j].is_falling) {
				PIECES(i - top_left.x, j - top_left.y) = board->cells[i][j];
			}
		}
#undef PIECES

	float complex top_left_comp = top_left.x + I * top_left.y;
	float complex center = ((2 * top_left.x + width) / 2) + I * ((2 * top_left.y + height) / 2);
	float complex top_left_dest = (top_left_comp - center) * I + center;

	vec2i new_top_left = { crealf(top_left_dest) - 2, cimagf(top_left_dest) + 2 };
	vec2i new_bottom_right = { new_top_left.x + height, new_top_left.y + width };

	if (new_top_left.x < 0 || new_top_left.x >= BOARD_WIDTH || new_top_left.y < 0 || new_top_left.y >= BOARD_HEIGHT ||
		new_bottom_right.x < 0 || new_bottom_right.x >= BOARD_WIDTH || new_bottom_right.y < 0 || new_bottom_right.y >= BOARD_HEIGHT) {
		free(pieces);
		return NULL;
	}

	if (tl != NULL) {
		*tl = new_top_left;
	}
	if (dimensions != NULL) {
		*dimensions = (vec2i){ width, height };
	}
	return pieces;
}
bool Board_can_rotate_falling_blocks(const Board *board) {
	assert(board);

	vec2i top_left;
	vec2i dimensions;
	Cell *cells = get_new_top_left(board, &top_left, &dimensions);
	if (cells == NULL) {
		return false;
	} else {
		free(cells);
		return true;
	}
}
void Board_rotate_falling_blocks(Board *board) {
	assert(board);
	assert(Board_can_rotate_falling_blocks(board));
	vec2i top_left;
	vec2i dimensions;

	Cell *pieces = get_new_top_left(board, &top_left, &dimensions);
	if (pieces == NULL) {
		return;
	}
	const int32_t width = dimensions.x;
	const int32_t height = dimensions.y;


	for (int32_t j = 0; j < BOARD_HEIGHT; ++j)
		for (int32_t i = 0; i < BOARD_WIDTH; ++i) {
			if (board->cells[i][j].is_falling) {
				board->cells[i][j] = CreateCell();
			}
		}

	//vec2i center = { (2 * top_left.x + width) / 2, (2 * top_left.y + height) / 2 };

	assert(width > 0 && height > 0);
#define PIECES(i, j) pieces[(i) + (j) * width]
	for (int32_t j = 0; j < height; ++j)
		for (int32_t i = 0; i < width; ++i) {
			if (PIECES(i, j).is_falling) {
				PIECES(i, j).rotation = (PIECES(i, j).rotation + 1) % 4;
				swap_cells(&PIECES(i, j), &board->cells[top_left.x + j][top_left.y - i]);
			}
		}
#undef PIECES
	free(pieces);
}
void Board_draw(const Board *board, SDL_Renderer *renderer) {
	assert(board);
	assert(renderer);

	for (int y = 0; y < BOARD_HEIGHT; ++y)
		for (int x = 0; x < BOARD_WIDTH; ++x) {
			CellDraw(&board->cells[x][y], renderer, (SDL_Point){ x, y - BOARD_OFFSET });
		}
}
