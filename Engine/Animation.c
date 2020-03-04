#include <assert.h>
#include "Animation.h"

Animation CreateAnimation(uint32_t size, const SDL_Rect *rects, float fps) {
    assert(rects);
    assert(size != 0);

    Animation anim = {
        .rects = malloc(sizeof(SDL_Rect) * size),
        .n = size,
        .fps = fps
    };
    memcpy(&anim.rects, rects, sizeof(SDL_Rect) * size);
    return anim;
}
void DestroyAnimation(Animation *anim) {
    assert(anim);

    free(anim->rects);
    anim->n = 0;
    anim->fps = 0.0f;
}

AnimMap CreateAnimMap() {
    return (AnimMap) {
        .map = CreateMap(sizeof(Animation), (void (*)(void*))DestroyAnimation)
    };
}
void DestroyAnimMap(AnimMap *map) {
    assert(map);

    DestroyMap(&map->map);
}
void InsertAnimMap(AnimMap *map, const char *name, Animation animation) {
    assert(map);
    assert(name);
    assert(animation.n != 0);
    assert(animation.rects != NULL);
    InsertElemMap(&map->map, name, &animation);
}
int64_t GetIndexAnimMap(AnimMap *map, const char *name) {
    assert(map);
    assert(name);
    return GetIndexMap(&map->map, name);
}
Animation* GetAnimMap(AnimMap *map, int64_t index) {
    assert(map);
    if (index == -1) {
        return NULL;
    }
    assert(0 <= index && index < map->map.size);
    return (Animation*)GetElemMap(&map->map, index);
}
Animation* GetAnimFromName(AnimMap *map, const char *name) {
    assert(map);
    assert(name);
    return GetAnimMap(map, GetIndexAnimMap(map, name));
}
AnimManager CreateAnimManager(AnimMap anim_map) {
    return (AnimManager) {
        .map = anim_map,
        .curr_anim_index = -1,
        .curr_frame = 0,
        .time_elasped = 0.0f
    };
}
void DestroyAnimManager(AnimManager *manager) {
    assert(manager);

    DestroyAnimMap(&manager->map);
    manager->curr_anim_index = -1;
    manager->curr_frame = 0;
    manager->time_elasped = 0.0f;
}
void SetCurrentAnim(AnimManager *manager, const char *name) {
    assert(manager);
    assert(name);

    manager->curr_anim_index = GetIndexAnimMap(&manager->map, name);
    manager->curr_frame = 0;
}
Animation* GetCurrentAnim(AnimManager *manager) {
    assert(manager);

    if (manager->curr_anim_index == -1) {
        return NULL;
    }
    return GetAnimMap(&manager->map, manager->curr_anim_index);
}
SDL_Rect GetCurrentFrame(AnimManager *manager) {
    assert(manager);
    Animation* anim = GetCurrentAnim(manager);
    if (anim == NULL) {
        return (SDL_Rect){ 0, 0, 0, 0 };
    }
    assert(0 <= manager->curr_frame && manager->curr_frame < anim->n);

    return anim->rects[manager->curr_frame];
}
void UpdateAnimManager(AnimManager *manager, uint32_t time_elasped_ms) {
    assert(manager);

    Animation *anim = GetCurrentAnim(manager);
    if (anim == NULL) {
        return;
    }

    manager->time_elasped += time_elasped_ms;
    const uint32_t ms_per_frame = 1000 / anim->fps;
    manager->curr_frame += manager->time_elasped / ms_per_frame;
    manager->curr_frame %= anim->n;
    manager->time_elasped = manager->time_elasped % ms_per_frame;
}