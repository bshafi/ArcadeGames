#pragma once

#include "../Engine/vec2i.h"

typedef enum TetrominoType TetrominoType;
enum TetrominoType {
	TETROMINO_O = 0,
	TETROMINO_Z = 1,
	TETROMINO_J = 2,
	TETROMINO_S = 3,
	TETROMINO_L = 4,
	TETROMINO_T = 5,
	TETROMINO_I = 6
};

#define BLOCK_LENGTH 8
#define TETRA 4

extern const SDL_Rect tetromino_source_rects[7][TETRA];

SDL_Point GetBottomRight(TetrominoType type);
SDL_Point GetTopLeft(TetrominoType type);