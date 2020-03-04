#include <assert.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Connect4Board.h"
#include "GUI.h"
#include "../Engine/vec2i.h"

void CreateGUI(struct GUI *gui, SDL_Renderer *renderer) {
    assert(gui != NULL);
    assert(renderer != NULL);

    gui->background = IMG_LoadTexture(renderer, "resources/Connect4Board.png");

    assert(gui->background != NULL);
}
void RenderGUI(struct GUI *gui, SDL_Renderer *renderer, const struct Connect4 *game) {
    assert(gui != NULL);
    assert(renderer != NULL);
    assert(game != NULL);

    const SDL_Color WHITE  = (SDL_Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
    const SDL_Color RED   = (SDL_Color){ .r = 255, .g =   0, .b =   0, .a = 255 };
    const SDL_Color GREEN = (SDL_Color){ .r =   0, .g = 255, .b =   0, .a = 255 };

    SDL_Rect viewport;
    SDL_RenderGetViewport(renderer, &viewport);
    int pieceWidth = viewport.w / BOARD_WIDTH;
    int pieceHeight = viewport.h / BOARD_HEIGHT;

    for (int x = 0; x < BOARD_WIDTH; ++x)
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            const enum Player piece = GetPieceAtPosConnect4(game, x, y);
            const SDL_Rect dest = {
                .x = (pieceWidth * x),
                .y = viewport.h - (pieceHeight * (y + 1)),
                .w = pieceWidth,
                .h = pieceHeight,
            };

            switch (piece) {
            case PlayerNone:
                SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
                break;
            case PlayerRed:
                SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
                break;
            case PlayerGreen:
                SDL_SetRenderDrawColor(renderer, GREEN.r, GREEN.g, GREEN.b, GREEN.a);
                break;
            default:
                assert(false);
            }
            
            SDL_RenderFillRect(renderer, &dest);
        }

    SDL_RenderCopy(renderer, gui->background, &viewport, &viewport);
    
    if (game->winner != PlayerNone) {
        Line line = {
            .p0 = {
                .x = (game->winLine.p0.x) * pieceWidth + (pieceWidth / 2),
                .y = (BOARD_HEIGHT - game->winLine.p0.y - 1) * pieceHeight + (pieceHeight / 2)
            },
            .p1 = {
                .x = (game->winLine.p1.x) * pieceWidth + (pieceWidth / 2),
                .y = (BOARD_HEIGHT - game->winLine.p1.y - 1) * pieceHeight + (pieceHeight / 2)
            }
        };

        SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
        SDL_RenderDrawLine(renderer, line.p0.x, line.p0.y, line.p1.x, line.p1.y);
    }
}
void DestroyGUI(struct GUI *gui) {
    assert(gui != NULL);

    SDL_DestroyTexture(gui->background);
}