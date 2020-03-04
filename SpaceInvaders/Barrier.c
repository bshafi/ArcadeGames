#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../Engine/App.h"

#include "Barrier.h"
#include "Lazer.h"

const bool BARRIER_DATA[BARRIER_HEIGHT][BARRIER_WIDTH] = {
    { 0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },
    { 0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0 },
    { 0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0 },
    { 0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0 },
    { 0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0 }
};

Barrier CreateBarrier(vec2i pos) {
    Barrier barrier;
    barrier.pos = pos;
    memcpy(&barrier.data[0], &BARRIER_DATA[0][0], sizeof(BARRIER_DATA));
    return barrier;
}


Barriers CreateBarriers() {
    Barriers barriers;
    for (int i = 0; i < BARRIERS_COUNT; ++i) {
        vec2i pos = { 47 + (46 * i), 167 };
        barriers.barriers[i] = CreateBarrier(pos);
    }
    return barriers;
}

bool GetBarrierPiece(const Barrier *barrier, uint32_t i, uint32_t j) {
    assert(barrier);
    assert(i < BARRIER_WIDTH);
    assert(j < BARRIER_HEIGHT);

    return barrier->data[j][i];
}
void print_binary(uint8_t a) {
    for (uint32_t i = 0; i < sizeof(a); ++i) {
        fprintf(stderr, "%c", ((a << (7 - i)) >> i) ? '1' : '0');
    }
    fprintf(stderr, "\n");
}
void SetBarrierPiece(Barrier *barrier, uint32_t i, uint32_t j, bool t) {
    assert(barrier);
    assert(i < BARRIER_WIDTH);
    assert(j < BARRIER_HEIGHT);

    barrier->data[j][i] = t;
}
bool BarrierHasCollision(const Barrier *barrier, const Lazer *lazer, SDL_Point *point_rel_to_barrier) {
    assert(barrier);
    assert(lazer);

    for (int j = 0; j < LAZER_HEIGHT; ++j)
        for (int i = 0; i < LAZER_WIDTH; ++i) {
            int x = i + lazer->pos.x - barrier->pos.x;
            int y = j + lazer->pos.y - barrier->pos.y;

            if (x < 0 || x >= BARRIER_WIDTH) {
                continue;
            }
            if (y < 0 || y >= BARRIER_HEIGHT) {
                continue;
            }
            
            if (GetBarrierPiece(barrier, x, y)) {
                if (point_rel_to_barrier != NULL) {
                    *point_rel_to_barrier = (SDL_Point){ x, y };
                }
                return true;
            }
        }
    return false;
}
void BarrierHandleColision(Barrier *barrier, const Lazer *lazer) {
    assert(barrier);
    assert(lazer);

    SDL_Point point_rel_to_barrier;
    if (!BarrierHasCollision(barrier, lazer, &point_rel_to_barrier)) {
        return;
    }

    int dir = lazer->direction == LAZER_UP ? -1 : 1;
    const int delta = dir * 5 + point_rel_to_barrier.y;
    int i = point_rel_to_barrier.x;
    for (int j = point_rel_to_barrier.y; j < delta; j += dir) {
        if (i < 0 || i >= BARRIER_WIDTH || j < 0 || j >= BARRIER_HEIGHT) {
            continue;
        }

        SetBarrierPiece(barrier, i, j, false);

        if (0 <= i + 1 && i + 1 < BARRIER_WIDTH) {
            SetBarrierPiece(barrier, i, j, false);
        }
        if (0 <= i - 1 && i - 1 < BARRIER_WIDTH) {
            SetBarrierPiece(barrier, i, j, false);
        }
    }
}
void DrawBarrier(const Barrier *barrier, SDL_Renderer *renderer) {
    assert(barrier);
    assert(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect barrier_rect = { barrier->pos.x, barrier->pos.y, BARRIER_WIDTH, BARRIER_HEIGHT };
    SDL_RenderFillRect(renderer, &barrier_rect);

    SDL_Point points[BARRIER_WIDTH * BARRIER_HEIGHT];
    int points_size = 0;
    for (int j = 0; j < BARRIER_HEIGHT; ++j)
        for (int i = 0; i < BARRIER_WIDTH; ++i) {
            if (GetBarrierPiece(barrier, i, j)) {
                points[points_size] = (SDL_Point){ barrier->pos.x + i, barrier->pos.y + j };
                ++points_size;
                assert(points_size < BARRIER_WIDTH * BARRIER_HEIGHT);
            }
        }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoints(renderer, &points[0], points_size);
}