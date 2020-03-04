#pragma once

#include <stdbool.h>

#include "SDL2/SDL_rect.h"

#include "Text.h"
#include "EventArray.h"
#include "vec2i.h"

struct SDL_Renderer;
struct SDL_Texture;

typedef struct Button Button;


struct Button {
    SDL_Rect rect;
    SDL_Texture *texture;
    bool clicked;
    SDL_Color backgroundColor;
};

Button CreateButton(SDL_Renderer *renderer, const Text *text, SDL_Rect rect, SDL_Color backgroundColor);
void DestroyButton(Button *button);

void RenderButton(const Button *button, SDL_Renderer *renderer);
void UpdateButton(Button *button, const EventArray *event_arr);