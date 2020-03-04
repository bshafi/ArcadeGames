#include "vec2i.h"

SDL_Rect SDL_ExpandAToFitInB(const SDL_Rect *a, const SDL_Rect *b) {
    float x_scale = (float)b->w / (float)a->w;
    float y_scale = (float)b->h / (float)a->h;

    float scale = x_scale < y_scale ? x_scale : y_scale;

    SDL_Rect ret = {
        .x = (b->w - (a->w * scale)) / 2,
        .y = (b->h - (a->h * scale)) / 2,
        .w = a->w * scale,
        .h = a->h * scale
    };
    return ret;
}

int int_max(int a, int b) {
	return (a > b) ? a : b;
}
int int_min(int a, int b) {
	return (a < b) ? a : b;
}