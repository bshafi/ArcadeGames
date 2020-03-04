#pragma once

#include "../Engine/vec2i.h"

#include "App.h"
#include "EventArray.h"

typedef struct Scene Scene;

struct SDL_Renderer;

typedef struct SceneFnTable SceneFnTable;
struct SceneFnTable {
    void (*CreateScene)(void *scene, App *app);
    void (*UpdateScene)(void *scene, const EventArray *event, uint32_t delta_time_ms);
    void (*RenderScene)(void *scene, App *app);
    void (*DestroyScene)(void *scene);
};

struct Scene {
    SceneFnTable const* table;
    void *scene_data;
};