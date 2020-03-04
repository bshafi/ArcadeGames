#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "TitleScreen.h"

const SceneFnTable TITLE_SCREEN_SCENE_TABLE = { CreateTitleScreen, UpdateTitleScreen, RenderTitleScreen, DestroyTitleScreen };

void CreateTitleScreenScene(TitleScreenScene *tss, App *app) {
    assert(tss != NULL);
    assert(app != NULL);
    Text text = {
        .text = "Play Connect 4",
        .textColor = (SDL_Color){ 0, 0, 0, 0 },
        .ptsize = 70
    };
    tss->button = CreateButton(
        app->renderer, 
        &text, 
        (SDL_Rect){ 160, 120, 320, 240 },
        (SDL_Color){ 255, 255, 255, 255}
    );
}
void DestroyTitleScreenScene(TitleScreenScene *tss) {
    assert(tss != NULL);
//    SDL_DestroyTexture(tss->playText);
    DestroyButton(&tss->button);
}

void UpdateTitleScreen(Scene *scene, const EventArray *event_arr, uint32_t delta_ms) {
    assert(scene != NULL);
    assert(event_arr != NULL);
    
    TitleScreenScene *tss = (TitleScreenScene*)scene->scene_data;

    UpdateButton(&tss->button, event_arr);
    if (tss->button.clicked) {
        tss->button.clicked = false;
        PushSceneChangeEvent("board");
    }
/*
    for (int i = 0; i < event_arr->n; ++i) {
        const SDL_Event event = event_arr->event[i];
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            SDL_Rect mouseRect = {
                .x = event.button.x,
                .y = event.button.y,
                .w = 1,
                .h = 1
            };
            if (SDL_HasIntersection(&mouseRect, &tss->buttonRect)) {
                SDL_Event event = {0};
                event.type = USEREVENT_TYPE;
                event.user.code = 0;
                event.user.data1 = strdup("board");
                event.user.data2 = NULL;
                int err = SDL_PushEvent(&event);
                assert(err != -1);
            }
        }
    }
*/
}
void RenderTitleScreen(Scene *scene, App *app) {
    assert(scene != NULL);
    assert(app != NULL);

    TitleScreenScene *tss = (TitleScreenScene*)scene->scene_data;

    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0x00, 0xff);
//    SDL_RenderFillRect(renderer, &tss->button.rect);
//    SDL_RenderCopy(renderer, tss->playText, NULL, &tss->buttonRect);
    RenderButton(&tss->button, app->renderer);
}


void CreateTitleScreen(Scene *scene, App *app) {
    assert(scene != NULL);
    assert(app != NULL);
    
    scene->scene_data = malloc(sizeof(TitleScreenScene));
    
    TitleScreenScene *tss = (TitleScreenScene*)scene->scene_data;

    CreateTitleScreenScene(tss, app);
}
void DestroyTitleScreen(Scene *scene) {
    assert(scene != NULL);
    
    TitleScreenScene *tss = (TitleScreenScene*)scene->scene_data;

    DestroyTitleScreenScene(tss);
    free(scene->scene_data);
}