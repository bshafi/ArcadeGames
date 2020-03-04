#pragma once

#include "vec2i.h"

struct SDL_Renderer;
struct SDL_Window;
struct SDL_Texture;

typedef struct App App;

struct App {
    SDL_Window *window;
    SDL_Renderer *renderer;
    float scale;
    vec2i offset;
};

App CreateApp(const char *name, const size_t width, const size_t height);
void DestroyApp(App *app);

void SetVirtualWindow(App *app, size_t width, size_t height);
void RenderApp(App *app, SDL_Texture *texture, const SDL_Rect *src, const SDL_Rect *dest);

void SetAppCamera(App *app, const size_t w, const size_t h);
void TakeScreenShot(App *app);

void PushSceneChangeEvent(const char *scene_name);

/* DO NOT MODIFY */
uint32_t CHANGE_SCENE_EVENT;

/* DO NOT MODIFY */
uint32_t CREATE_LAZER_EVENT;

uint32_t USEREVENT_TYPE;

uint32_t GAMEOVER_EVENT;