#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Connect4Board.h"
#include "../Engine/vec2i.h"

int CountPiecesInLine(struct Connect4 *game, int x, int y, int deltaX, int deltaY, Line *line) {
    assert(game != NULL);

    int count = 1;
    for (
        int xMin = x, yMin = y;;
        ++count,
        xMin -= deltaX,
        yMin -= deltaY 
    ) { 
        if (
            0 <= (xMin - deltaX) && (xMin - deltaX) < BOARD_WIDTH &&
            0 <= (yMin - deltaY) && (yMin - deltaY) < BOARD_HEIGHT &&
            GetPieceAtPosConnect4(game, (xMin - deltaX), (yMin - deltaY)) == game->currentPlayer
        ) {
            continue;
        } else {
            if (line != NULL) {
                line->p0.x = xMin;
                line->p0.y = yMin;
            }
            break;
        }
    }
    for (
        int xMax = x, yMax = y;;
        ++count,
        xMax += deltaX,
        yMax += deltaY 
    ) { 
        if (
            0 <= (xMax + deltaX) && (xMax + deltaX) < BOARD_WIDTH &&
            0 <= (yMax + deltaY) && (yMax + deltaY) < BOARD_HEIGHT &&
            GetPieceAtPosConnect4(game, (xMax + deltaX), (yMax + deltaY)) == game->currentPlayer
        ) {
            continue;
        } else {
            if (line != NULL) {
                line->p1.x = xMax;
                line->p1.y = yMax;
            }
            break;
        }
    }

    return count;
}

// places a piece at a given (x, y) and changes player turn
// determines if there is a winner and sets the value of winner if there is one
void SetPieceAtPos(struct Connect4 *game, int x, int y) {
    assert(game != NULL);
    assert(GetPieceAtPosConnect4(game, x, y) == PlayerNone);

    game->board[x][y] = game->currentPlayer;
    Line line = { .p0 = { 0, 0 }, .p1 = { 0, 0 } };
    if (CountPiecesInLine(game, x, y, 1, 0, &line) >= 4) {
        game->winLine = line;
        game->winner = game->currentPlayer;
    } else if (CountPiecesInLine(game, x, y, 0, 1, &line) >= 4) {
        game->winLine = line;
        game->winner = game->currentPlayer;
    } else if (CountPiecesInLine(game, x, y, 1,  1, &line) >= 4) {
        game->winLine = line;
        game->winner = game->currentPlayer;
    } else if (CountPiecesInLine(game, x, y, 1, -1, &line) >= 4) {
        game->winLine = line;
        game->winner = game->currentPlayer;
    }

    if (game->currentPlayer == PlayerRed) {
        game->currentPlayer = PlayerGreen;
    } else {
        game->currentPlayer = PlayerRed;
    }
}

void CreateConnect4(struct Connect4 *game) {
    assert(game != NULL);

    // randomly select the first player
    if (rand() % 2 == 0) {
        game->currentPlayer = PlayerRed;
    } else {
        game->currentPlayer = PlayerGreen;
    }

    game->winner = PlayerNone;
    game->winLine = (Line){ .p0 = { 0, 0 }, .p1 = { 0, 0 } };

    // initializes all the tiles to empty
    memset(game->board, PlayerNone, sizeof(game->board));
}
void PlacePieceConnect4(struct Connect4 *game, int pos) {
    assert(game != NULL);
    assert(game->winner == PlayerNone);
    assert(0 <= pos && pos <= BOARD_WIDTH);

    const int x = pos;
    for (int y = BOARD_HEIGHT; y >= 0; --y) {
        // if there are no pieces in this column
        // put a piece at the bottom
        if (y == 0) {
            SetPieceAtPos(game, x, y);
            return;
        }

        const enum Player space = GetPieceAtPosConnect4(game, x, y - 1);

        // if there is no space left to put a piece
        if (y == BOARD_HEIGHT && space != PlayerNone) {
            return;

        // if y is within the board and the space below y is filled
        // put a piece in y and switch the current player
        } else if (y < BOARD_HEIGHT && space != PlayerNone) {
            SetPieceAtPos(game, x, y);
            return;

        // if there is space below y go down and check again
        } else {
            continue;
        }
    }
}
enum Player WhoWonConnect4(const struct Connect4 *game) {
    assert(game != NULL);
    return game->winner;
}

const enum Player GetPieceAtPosConnect4(const struct Connect4 *game, int x, int y) {
    assert(game != NULL);
    assert(0 <= x && x <= BOARD_WIDTH);
    assert(0 <= y && y <= BOARD_HEIGHT);

    return game->board[x][y];
}