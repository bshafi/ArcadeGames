#pragma once

#include "SDL2/SDL_rect.h"

struct SDL_Texture;
struct SDL_Renderer;

typedef struct Text Text;

struct Text {
    const char *text;
    SDL_Color textColor;
    int ptsize;
};


SDL_Texture *CreateTextureFromText(SDL_Renderer *renderer, const char* fontFilePath, const Text *text);