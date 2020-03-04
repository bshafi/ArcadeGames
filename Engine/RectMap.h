#pragma once

#include <stdint.h>

#include <SDL2/SDL_rect.h>

struct SDL_Texture;
struct FILE;

typedef struct RectMap RectMap;

struct RectMap {
    char **names;
    SDL_Rect *rects;
    size_t n, capacity;
};

RectMap CreateRectMap(const size_t initial_capacity);
void DestroyRectMap(RectMap *sr_map);

void PushRectMap(RectMap *r_map, const char *name, const SDL_Rect rect);
SDL_Rect GetFromRectMap(const RectMap *r_map, const char *name);

void RemoveRectFromRectMap(RectMap *r_map, const SDL_Rect);
SDL_Rect GetIntersectingRectFromRectMap(const RectMap *r_map, const SDL_Rect);

RectMap ReadRectMapFromFile(const char *);
void WriteRectMapToFile(const char *, RectMap *r_map);