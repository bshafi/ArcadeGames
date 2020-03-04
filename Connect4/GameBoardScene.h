#pragma once

#include "GUI.h"
#include "Connect4Board.h"
#include "../Engine/Scene.h"

struct SDL_Renderer;
struct EventArray;

typedef struct GameBoardSceneData GameBoardSceneData; 
struct GameBoardSceneData {
    Connect4 board;
    GUI gui;
    SDL_Renderer *renderer;
};

void CreateGameBoardSceneData(GameBoardSceneData *gbsd, SDL_Renderer *renderer);
void DestroyGameBoardSceneData(GameBoardSceneData *gbsd);

void CreateGameBoard(Scene *scene, App *app);
void UpdateGameBoard(Scene *scene, const EventArray *event_arr, uint32_t delta_time_ms);
void RenderGameBoard(Scene *scene, App *);
void DestroyGameBoard(Scene *scene);

extern const SceneFnTable GAME_BOARD_SCENE_TABLE;