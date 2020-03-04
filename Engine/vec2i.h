#pragma once

#include <SDL2/SDL_rect.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct vec2i vec2i;

struct vec2i {
    int32_t x, y;
};

typedef struct vec2f vec2f;
struct vec2f {
    float x, y;
};

typedef struct Transform Transform;
struct Transform {
    vec2f pos;
    vec2f scale;
};

typedef struct Line Line;

struct Line {
    vec2i p0, p1;
};


int int_max(int a, int b);
int int_min(int a, int b);
typedef SDL_Rect Rect;

SDL_Rect SDL_ExpandAToFitInB(const SDL_Rect *a, const SDL_Rect *b);

#define STRUCT(NAME) \
typedef struct NAME NAME; \
struct NAME
