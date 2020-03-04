#pragma once

#include "Map.h"

typedef struct Animation Animation;
struct Animation {
    SDL_Rect *rects;
    uint32_t n;
    float fps;
};

Animation CreateAnimation(uint32_t size, const SDL_Rect *rects, float fps);
void DestroyAnimation(Animation *);

typedef struct AnimMap AnimMap;
struct AnimMap {
    Map map;
};

typedef struct AnimManager AnimManager;
struct AnimManager {
    AnimMap map;
    int64_t curr_anim_index;
    uint32_t curr_frame;
    uint32_t time_elasped;
};

AnimMap CreateAnimMap();
void DestroyAnimMap(AnimMap *map);
void InsertAnimMap(AnimMap *map, const char *name, Animation animation);
int64_t GetIndexAnimMap(AnimMap *map, const char *name);
Animation* GetAnimMap(AnimMap *map, int64_t index);
Animation* GetAnimFromName(AnimMap *map, const char *name);

AnimManager CreateAnimManager(AnimMap map);
void DestroyAnimManager(AnimManager *manager);
void SetCurrentAnim(AnimManager *manager, const char *name);
Animation* GetCurrentAnim(AnimManager *manager);
SDL_Rect GetCurrentFrame(AnimManager *manager);
void UpdateAnimManager(AnimManager *manager, uint32_t time_elasped_ms);