#include <assert.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../Engine/App.h"

#include "Lazer.h"

#define ORIGIN (vec2i){ 0, 0 }


const char *LazerTypeToStr(LazerType lazer_type) {
    switch (lazer_type) {
        case LAZER_SLIDE:
            return "lazer_slide";
        case LAZER_ZIG:
            return "lazer_zig";
        case LAZER_BARB:
            return "lazer_barb";
        default:
            return NULL;
    }
}

Lazers CreateLazers() {
    Lazers lazers;
    
    for (int i = 0; i < LAZER_COUNT; ++i) {
        lazers.lazers[i].frame = 0;
        lazers.lazers[i].type = LAZER_NONE;
        lazers.lazers[i].pos = ORIGIN;
    }

    return lazers;
}
void UpdateLazers(Lazers *lazers, const uint32_t delta_time_ms) {
    assert(lazers);

    lazers->ms_elasped += delta_time_ms;
    lazers->ms_elasped += delta_time_ms;
#define PERIOD (10)
    if (lazers->ms_elasped > PERIOD) {
        lazers->ms_elasped -= PERIOD;
    } else {
        return;
    }

    for (int i = 0; i < LAZER_COUNT; ++i) {
        Lazer *lazer = &lazers->lazers[i];
        if (lazer->pos.x < 0 || lazer->pos.y < 0 || lazer->pos.x > 256 || lazer->pos.y > 224) {
            lazer->type = LAZER_NONE;
        } 
        if (lazer->type == LAZER_NONE) {
            continue;
        }

        const int8_t direction = lazer->direction ? 1 : -1;
        lazer->pos.y += delta_time_ms * (0.1f * direction);
        ++lazer->frame;
        if (lazer->frame > 3) {
            lazer->frame = 0;
        }
    }

}
void AddLazer(Lazers *lazers, Lazer lazer) {
    assert(lazers);

    for (int i = 0; i < LAZER_COUNT; ++i)
        if(lazers->lazers[i].type == LAZER_NONE) {
            lazers->lazers[i] = lazer;
            return;
        }
}