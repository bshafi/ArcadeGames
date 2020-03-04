#include <assert.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "../Engine/Scene.h"
#include "../Engine/App.h"
#include "GameBoardScene.h"

const SceneFnTable GAME_BOARD_SCENE_TABLE = { CreateGameBoard, UpdateGameBoard, RenderGameBoard, DestroyGameBoard };

void CreateGameBoardSceneData(GameBoardSceneData *gbsd, SDL_Renderer *renderer) {
    assert(gbsd != NULL);
    assert(renderer != NULL);

    CreateConnect4(&gbsd->board);
    CreateGUI(&gbsd->gui, renderer);
    gbsd->renderer = renderer;
}
void DestroyGameBoardSceneData(GameBoardSceneData *gbsd) {
    assert(gbsd != NULL);
    DestroyGUI(&gbsd->gui);
}
void UpdateGameBoard(Scene *scene, const EventArray *event_arr, uint32_t delta_time_ms) {
    assert(scene != NULL);
    assert(event_arr != NULL);

    GameBoardSceneData *gbsd = scene->scene_data;
    SDL_Rect viewport;
    SDL_RenderGetViewport(gbsd->renderer, &viewport);
    const int WINDOW_WIDTH = viewport.w;

    Connect4 *board = &gbsd->board;
    for (int i = 0; i < event_arr->n; ++i) {
            SDL_Event event = event_arr->event[i];

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (board->winner == PlayerNone) {
                    int xPos;
                    SDL_GetMouseState(&xPos, NULL);
                    int xBoardPos = xPos / (WINDOW_WIDTH / BOARD_WIDTH);
                    PlacePieceConnect4(board, xBoardPos);
                } else {
                    CreateConnect4(board);
                }
            }
        }
}
void RenderGameBoard(Scene *scene, App *app) {
    assert(scene != NULL);
    assert(app != NULL);

    GameBoardSceneData *gbsd = scene->scene_data;
    GUI *gui = &gbsd->gui;
    Connect4 *board = &gbsd->board;
    RenderGUI(gui, app->renderer, board);
}


void CreateGameBoard(Scene *scene, App *app) {
    assert(scene != NULL);
    assert(app != NULL);

    scene->scene_data = malloc(sizeof(GameBoardSceneData));
    CreateGameBoardSceneData(scene->scene_data, app->renderer);
}
void DestroyGameBoard(Scene *scene) {
    assert(scene != NULL);

    DestroyGameBoardSceneData(scene->scene_data);
    free(scene->scene_data);
}