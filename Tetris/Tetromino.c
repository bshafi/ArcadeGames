#include "Tetromino.h"

const SDL_Rect tetromino_source_rects[7][TETRA] = {
	{
		{ 179, 16, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 187, 16, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 179, 24, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 187, 24, BLOCK_LENGTH, BLOCK_LENGTH },
	},
	{
		{ 204, 16, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 212, 16, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 212, 24, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 220, 24, BLOCK_LENGTH, BLOCK_LENGTH },
	},
	{
		{ 238, 15, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 246, 15, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 254, 15, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 254, 23, BLOCK_LENGTH, BLOCK_LENGTH },
	},
	{
		{ 271, 23, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 279, 23, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 279, 15, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 287, 15, BLOCK_LENGTH, BLOCK_LENGTH },
	},
	{
		{ 179, 40, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 187, 40, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 195, 40, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 179, 48, BLOCK_LENGTH, BLOCK_LENGTH },
	},
	{
		{ 213, 40, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 221, 40, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 229, 40, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 221, 48, BLOCK_LENGTH, BLOCK_LENGTH },
	},
	{
		{ 244, 46, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 252, 46, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 260, 46, BLOCK_LENGTH, BLOCK_LENGTH },
		{ 268, 46, BLOCK_LENGTH, BLOCK_LENGTH },
	},
};

SDL_Point GetBottomRight(TetrominoType type) {
	const SDL_Rect *source_rects = &tetromino_source_rects[(uint32_t)(type)][0];
	SDL_Point bottom_right = { source_rects[0].x + source_rects[0].w, source_rects[0].y + source_rects[0].h };
	for (int i = 1; i < TETRA; ++i) {
		bottom_right.x = int_max(bottom_right.x, source_rects[i].x + source_rects[i].w);
		bottom_right.y = int_max(bottom_right.y, source_rects[i].y + source_rects[i].h);
	}
	return bottom_right;
}
SDL_Point GetTopLeft(TetrominoType type) {
	const SDL_Rect *source_rects = &tetromino_source_rects[(uint32_t)(type)][0];
	SDL_Point top_left = { source_rects[0].x, source_rects[0].y };
	for (int i = 1; i < TETRA; ++i) {
		top_left.x = int_min(top_left.x, source_rects[i].x);
		top_left.y = int_min(top_left.y, source_rects[i].y);
	}
	return top_left;
}