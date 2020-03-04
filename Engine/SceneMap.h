#pragma once

#include <stdint.h>

typedef struct SceneMap SceneMap;
typedef char * cstr;

struct Scene;

struct SceneMap {
    cstr *names;
    Scene *scenes;
    int n;
};

SceneMap CreateSceneMap();
void DestroySceneMap(SceneMap *scene_map);
void CreateAllScenesSceneMap(SceneMap *scene_map, App *app);

void AddSceneToSceneMap(SceneMap *scene_map, cstr name, const Scene scene);
int GetSceneIndex(SceneMap *scene_map, const char *name);
Scene *GetSceneFromIndex(SceneMap *scene_map, int index);