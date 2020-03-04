#pragma once

#include "../Engine/vec2i.h"

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

typedef enum Player Player;
typedef struct Connect4 Connect4;

enum Player {
    PlayerNone,
    PlayerRed,
    PlayerGreen,
};

struct Connect4 {
    enum Player winner;
    enum Player currentPlayer;
    enum Player board[BOARD_WIDTH][BOARD_HEIGHT];
    Line winLine;
};

void CreateConnect4(struct Connect4 *game);
void PlacePieceConnect4(struct Connect4 *game, int pos);
enum Player WhoWonConnect4(const struct Connect4 *game);
const enum Player GetPieceAtPosConnect4(const struct Connect4 *game, int x, int y);