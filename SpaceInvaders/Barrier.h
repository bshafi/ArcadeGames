#pragma once

#include "../Engine/vec2i.h"
#include "../Engine/App.h"

struct SDL_Texture;
struct App;
struct Lazer;
typedef struct Lazer Lazer;
struct SDL_Renderer;


#define BARRIERS_COUNT 4
#define BARRIER_WIDTH 24
#define BARRIER_HEIGHT 16
#define BARRIER_BITS 16 * 3 * 8
#define BARRIER_BYTES BARRIER_BITS / 8

STRUCT (Barrier) {
    vec2i pos;
    bool data[BARRIER_HEIGHT][BARRIER_WIDTH];
};
Barrier CreateBarrier(vec2i pos);


STRUCT (Barriers) {
    Barrier barriers[BARRIERS_COUNT];
};

Barriers CreateBarriers();

bool GetBarrierPiece(const Barrier *barrier, uint32_t i, uint32_t j);
void SetBarrierPiece(Barrier *barrier, uint32_t i, uint32_t j, bool b);
void BarrierHandleColision(Barrier *barrier, const Lazer *lazer);
bool BarrierHasCollision(const Barrier *barrier, const Lazer *lazer, SDL_Point *point);
void DrawBarrier(const Barrier *barrier, SDL_Renderer *);