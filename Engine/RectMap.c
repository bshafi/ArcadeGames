#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"

#include "RectMap.h"


RectMap CreateRectMap(const size_t initial_capacity) {
    RectMap sr_map;
    if (initial_capacity > 0) {
        sr_map = (RectMap){
            .names = malloc(sizeof(char*) * initial_capacity),
            .rects = malloc(sizeof(SDL_Rect) * initial_capacity),
            .n = 0,
            .capacity = initial_capacity
        };
        
        assert(sr_map.names != NULL);
        assert(sr_map.rects != NULL);
    } else {
        sr_map = (RectMap){
            .names = NULL,
            .rects = NULL,
            .n = 0,
            .capacity = 0
        };
    }

    return sr_map;
}
void DestroyRectMap(RectMap *sr_map) {
    assert(sr_map != NULL);

    if (sr_map->capacity > 0) {
        free(sr_map->rects);
        for (int i = 0; i < sr_map->n; ++i) {
            free(sr_map->names[i]);
        }
        free(sr_map->names);
    }
    sr_map->capacity = 0;
    sr_map->n = 0;
    sr_map->names = NULL;
}

void PushRectMap(RectMap *r_map, const char *name, const SDL_Rect rect) {
    assert(r_map != NULL);
    assert(name != NULL);
    
    ++r_map->n;

    if (r_map->n > r_map->capacity) {
        if (r_map->capacity == 0) {
            r_map->capacity = 1;
        }
        r_map->capacity *= 2;
        r_map->names = realloc(r_map->names, sizeof(const char *) * r_map->capacity);
        r_map->rects = realloc(r_map->rects, sizeof(SDL_Rect) *r_map->capacity);
        assert(r_map->names);
        assert(r_map->rects);
    }

    const int last_index = r_map->n - 1;
    r_map->rects[last_index] = rect;
    r_map->names[last_index] = strdup(name);
}
SDL_Rect GetFromRectMap(const RectMap *r_map, const char *name) {
    assert(r_map != NULL);
    assert(name != NULL);

    for (int i = 0; i < r_map->n; ++i) {
        if (strcmp(name, r_map->names[i]) == 0) {
            return r_map->rects[i];
        }
    }
    return (SDL_Rect){ -1, -1, -1, -1 };
}

void RemoveRectFromRectMap(RectMap *r_map, const SDL_Rect rect) {
    assert(r_map != NULL);

    for (int i = 0; i < r_map->n;) {
        if (SDL_RectEquals(&rect, &r_map->rects[i])) {
            free(r_map->names[i]);
            --r_map->n;
            for (int j = i + 1; j < r_map->n; ++i) {
                r_map->names[j - 1] = r_map->names[j];
                r_map->rects[j - 1] = r_map->rects[j - 1];
            }
        } else {
            ++i;
        }
    }
}
SDL_Rect GetIntersectingRectFromRectMap(const RectMap *r_map, const SDL_Rect rect) {
    assert(r_map != NULL);

    for (int i = 0; i < r_map->n; ++i) {
        SDL_Rect r = r_map->rects[i];
        if (r.w == 0) {
            r.w = 1;
        }
        if (r.h == 0) {
            r.h = 1;
        }
        if (SDL_HasIntersection(&rect, &r)) {
            return r_map->rects[i];
        }
    }

    return (SDL_Rect){ -1, -1, -1, -1 };
}

RectMap ReadRectMapFromFile(const char *filepath) {
    assert(filepath != NULL);

    
    RectMap r_map = CreateRectMap(0);

    FILE *file = fopen(filepath, "rb+");
    char buffer[100];
    int rect_arr[4];
    while (!feof(file)) {
        fscanf(file, "%100s", buffer);
        fgetc(file); // ' ' char separator
        fread(rect_arr, sizeof(int) * 4, 1, file);

        SDL_Rect rect = { rect_arr[0], rect_arr[1], rect_arr[2], rect_arr[3] };
        PushRectMap(&r_map, buffer, rect);
    }
    fclose(file);

    return r_map;   
}
void WriteRectMapToFile(const char *filepath, RectMap *r_map) {
    assert(filepath != NULL);
    assert(r_map != NULL);

    FILE *file = fopen(filepath, "wb+");
    for (int i = 0; i < r_map->n; ++i) {
        fputs(r_map->names[i], file);
        fputc(' ', file);

        const SDL_Rect rect = r_map->rects[i];
        int rect_arr[4] = { rect.x, rect.y, rect.w, rect.h };
        fwrite(rect_arr, sizeof(int) * 4, 1, file); 
    }
    fclose(file);
}