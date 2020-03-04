#include "assert.h"
#include <string.h>

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"

#include "Scene.h"
#include "SceneMap.h"

SceneMap CreateSceneMap() {
    return (SceneMap) {
        .n = 0,
        .names = NULL,
        .scenes = NULL
    };
}
void DestroySceneMap(SceneMap *scene_map) {
    assert(scene_map != NULL);

    for (int i = 0; i < scene_map->n; ++i) {
        scene_map->scenes[i].table->DestroyScene(&scene_map->scenes[i]);
        free(scene_map->names[i]);
    }

    if (scene_map->n > 0) {
        free(scene_map->names);
        free(scene_map->scenes);
    }
    scene_map->n = 0;
    scene_map->names = NULL;
    scene_map->scenes = NULL;
}

void CreateAllScenesSceneMap(SceneMap *scene_map, App *app) {
    assert(scene_map != NULL);
    assert(app != NULL);
    for (int i = 0; i < scene_map->n; ++i) {
        scene_map->scenes[i].table->CreateScene(&scene_map->scenes[i], app);
    }
}

void AddSceneToSceneMap(SceneMap *scene_map, cstr name, const Scene scene) {
    assert(scene_map != NULL);
    assert(scene.scene_data == NULL);
    assert(name != NULL);

    ++scene_map->n;
    scene_map->names = realloc(scene_map->names, scene_map->n * sizeof(cstr));
    scene_map->scenes = realloc(scene_map->scenes, scene_map->n * sizeof(Scene));

    const int last_scene_index = scene_map->n - 1;
    scene_map->scenes[last_scene_index] = scene;
    scene_map->names[last_scene_index] = strdup(name);
}

int GetSceneIndex(SceneMap *scene_map, const char *name) {
    assert(scene_map != NULL);
    for (int i = 0; i < scene_map->n; ++i) {
        if (strcmp(name, scene_map->names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

Scene *GetSceneFromIndex(SceneMap *scene_map, int index) {
    assert(scene_map != NULL);
    assert(index >= 0);
    assert(index < scene_map->n);

    return &scene_map->scenes[index];
}