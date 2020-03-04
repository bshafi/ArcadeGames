#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "SDL2/SDL.h"

#include "../Connect4/GameBoardScene.h"
#include "../Connect4/TitleScreen.h"
#include "../Engine/Scene.h"
#include "../Engine/SceneMap.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FPS 15

int main() {
    App app = CreateApp("Connect4", WINDOW_WIDTH, WINDOW_HEIGHT);

    SceneMap scene_map = CreateSceneMap();

    AddSceneToSceneMap(&scene_map, "board", (Scene){ &GAME_BOARD_SCENE_TABLE, NULL });
    AddSceneToSceneMap(&scene_map, "title", (Scene){ &TITLE_SCREEN_SCENE_TABLE, NULL });

    CreateAllScenesSceneMap(&scene_map, &app);

    int scene_index = GetSceneIndex(&scene_map, "title");

    bool isRunning = true;
    EventArray window_events = CreateEventArray(),
        game_events = CreateEventArray(),
        user_events = CreateEventArray();

    uint32_t ticks = SDL_GetTicks();
    while (isRunning) {
        PollEvents(&window_events, SDL_QUIT, SDL_WINDOWEVENT);
        PollEvents(&game_events, SDL_KEYDOWN, SDL_MOUSEWHEEL);
        PollEvents(&user_events, SDL_USEREVENT, SDL_LASTEVENT);
    
        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

        for (int i = 0; i < user_events.n; ++i) {
            if (user_events.event[i].type == USEREVENT_TYPE) {
                const char *next_scene_name = user_events.event[i].user.data1;
                scene_index = GetSceneIndex(&scene_map, next_scene_name);
                free(user_events.event[i].user.data1);
                user_events.event[i].user.data1 = NULL;
            }
        }

        if (QueryEventArray(&window_events, NULL, ShouldQuit)) {
            isRunning = false;
        }

        Scene *curr_scene = GetSceneFromIndex(&scene_map, scene_index);
        curr_scene->table->UpdateScene(curr_scene, &game_events, 1000/FPS);

        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);

        curr_scene->table->RenderScene(curr_scene, &app);

        SDL_RenderPresent(app.renderer);

        const uint32_t delta_ticks = SDL_GetTicks() - ticks;
        if (delta_ticks < 1000/FPS) {
            SDL_Delay(1000/FPS - delta_ticks);
        }
        ticks = SDL_GetTicks();
    }

    DestroyEventArray(&user_events);
    DestroyEventArray(&game_events);
    DestroyEventArray(&window_events);

    DestroySceneMap(&scene_map);

    DestroyApp(&app);
}